"""Automated Gardening Project analytics engine.

A collection of miscellaneous functions for processing plant telemetry data,
as well as request-handling for analytics to be performed on behalf of other
parts of the system.
"""

from datetime import datetime, timedelta
import numpy as np
import json, os, random

def get_helper(func, data, key):
    """Helper function for get and argget.

    Gets a single result of an array function 'func' on a key 'key' of a list of
    dicts 'data', and the dict containing that result (if any). 'func' is
    performed by constructing an array of all of the values of 'key' in 'data'.

    Args:
        func: A NumPy function to be performed.
        data: A list of dicts containing data to perform a function on.
        key: A key in a datapoint or its telemetry dict to treat as the
            datapoint's value.

    Returns:
        A result of 'func' on 'key', and the dict in 'data' containing that
        result (if any).
    """

    # create an array from the values of key in data
    arr = np.empty(len(data))
    for i, d in enumerate(data):
        try:
            arr[i] = d[key]
        except KeyError:
            arr[i] = d["data"][key]

    # get result of func on arr, get argresult if is exists
    # argresult does not exist for some functions like np.mean, np.std, etc
    result = func(arr)
    try:
        index = np.where(arr == result)[0][0]
        argresult = data[index]

        # remove object ID (not json serialisable and not useful)
        del argresult["_id"]
    except IndexError:
        argresult = None

    return result, argresult

def get(func, data, key, count=1):
    """Gets results of an array function on a key of a list of dicts.

    Gets 'count' results of an array function 'func' on a key 'key' of a list of
    dicts 'data'. 'func' is performed by constructing an array of all of the
    values of 'key' in 'data'.

    Args:
        func: A NumPy function to be performed, as a string.
        data: A list of dicts containing data to perform a function on.
        key: A key in a datapoint or its telemetry dict to treat as the
            datapoint's value.
        count: The maximum number of results to return.

    Returns:
        A list of results of 'func' on 'key'.
    """

    func = eval(func)
    result, argresult = get_helper(func, data, key)

    # if count > 1 and an argresult exists, recursively call to construct list
    # requires an argresult because this is only meaningful for operations
    # ..which select array values, e.g. np.max and np.min
    # ..e.g. you can meaningfully select 5 max values, not 5 mean values
    if count > 1 and argresult != None:
        data.remove(argresult)
        return [result] + get(func, data, key, count=count-1)
    else:
        return [result]

def argget(func, data, key, count=1):
    """Gets dicts containing results of an array function on a key of a list of dicts.

    Gets 'count' dicts from a list of dicts 'data' containing results of
    an array function 'func' on a key 'key' of 'data'. 'func' is performed by
    constructing an array of all of the values of 'key' in 'data'.

    Args:
        func: A NumPy function to be performed, as a string.
        data: A list of dicts containing data to perform a function on.
        key: A key in a datapoint or its telemetry dict to treat as the
            datapoint's value.
        count: The maximum number of datapoints to return.

    Returns:
        A list of dicts containing datapoints containing results of 'func' on
        'key'.
    """

    func = eval(func)
    result, argresult = get_helper(func, data, key)

    if count > 1:
        data.remove(argresult)
        return [argresult] + argget(func, data, key, count=count-1)
    else:
        return [argresult]


def data_filter(func, data, key):
    """Filters a list of dicts given a key and a function for its value to pass.

    Gets all elements of a list of dicts 'data' for which key 'key' satisfies
    a boolean function 'func'.

    Args:
        func: A boolean function specifying a condition to be passed or failed.
        data: A list of dicts containing data to filter.
        key: A key in a datapoint or its telemetry dict to be tested.

    Returns:
        A list of dicts containing filtered data. Datapoints in 'data' which do
        not satisfy 'func' are omitted.
    """

    result = []
    for d in data:
        try:
            if func(d[key]):
                result.append(d)
        except KeyError:
            if func(d["data"][key]):
                result.append(d)

    return result

def aggregate(data, interval=5, mode=None, key=None):
    """Aggregates datapoints in common time intervals.

    Aggregates datapoints in 'data' in the same 'interval'-second time period
    according to 'mode' ("MAX", "MIN", or None). "MAX" and "MIN" take the
    maximum or minimum datapoint in each interval, while "None" averages
    the datapoints in each interval. If "MAX" or "MIN" is used, 'key' denotes
    the telemetry key to use for comparison.

    Args:
        data: A list of dicts containing data to aggregate.
        interval: A time period in seconds for which datapoints in the same
                interval should be aggregated.
        mode: A string (or NoneType) aggregation mode denoting how data should
            be aggregated.
        key: A key in the telemetry dict to use for comparison in MAX and MIN
            modes.

    Returns:
        A list of dicts containing aggregated data. Time and device data is
        taken from the first item in each interval, whereas telemetry data is
        aggregated according to the set aggregation mode.

    """

    # flip the list, as entries are sorted most to least recent and we want
    # the opposite
    data.reverse()

    start_time = data[0]["ts"]
    result = []

    # iterate through discrete, full intervals (stop if the remaining data
    # spans less than a complete interval).
    while data and data[-1]["ts"] - start_time >= interval:
        aggregate = {}
        aggregate["ts"] = data[0]["ts"]

        # if using default aggregation, initialise aggregate datapoint's
        # telemetry data as 0.
        if not mode:
            aggregate["data"] = {}
            for k in data[0]["data"]:
                aggregate["data"][k] = 0

        # otherwise, intialise it as the data from the first datapoint in the
        # aggregate.
        elif mode == "MAX" or mode == "MIN":
            aggregate["data"] = data[0]["data"]

        # iterate through each datapoint in the current interval
        i = 0
        for d in data:
            # break if the end of the current interval has been reached
            if d["ts"] - start_time > interval:
                break

            # if using default aggregation, sum each datapoints' values
            if not mode:
                for k, v in d["data"].items():
                    aggregate["data"][k] += v

            # otherwise, compare the value of 'key' found to the min/max so far
            elif mode == "MAX":
                if d["data"][key] > aggregate["data"][key]:
                    aggregate["data"] = d["data"]

            elif mode == "MIN":
                if d["data"][key] < aggregate["data"][key]:
                    aggregate["data"] = d["data"]

            # increment counter for the number of readings in this interval
            i += 1

        # if using default aggregation, divide each sum to get mean values
        if not mode:
            for k in d["data"]:
                aggregate["data"][k] /= i

        # add aggregate datapoint to result and remove interval from data
        result.append(aggregate)
        data = data[i+1:]

    return result

def anomaly_classify_by_key(stds, datum, data, key):
    """Classifies a data point as anomalous or not with respect to some attribute.

    Compares a data point 'datum' to historical data for the same device from
    the past hour, and returns True if telemetry key 'key' of 'datum' is more
    than 'stds' standard deviations away from the mean value of 'key' for the
    historical data 'data'.

    Args:
        stds: The number of standard deviations to test for.
        datum: A dict representing a telemetry reading to classify.
        data: A list of dicts containing historical data to compare to.
        key: A key in a datapoint or its telemetry dict to be tested.

    Returns:
        True if the data point is more than 'stds' standard deviations from the
        mean for data from the past hour for key 'key', or False otherwise.
    """

    data = data_filter(lambda x: x == datum["_id"], ts, "device")

    func = lambda x: abs(x - get(np.mean, data, key)[0]) > stds * get(np.std, data, key)[0]
    return func(d["data"][key])

def anomaly_classify(stds, datum, data):
    """Classifies a data point as anomalous or not with respect to all attributes.

    Compares a data point 'datum' to historical data for the same device from
    the past hour, and returns True if any telemetry key of 'datum' is more
    than 'stds' standard deviations away from the mean value of the same key for
    the historical data 'data'.

    Args:
        stds: The number of standard deviations to testsorry for.
        datum: A dict representing a telemetry reading to classify.
        data: A list of dicts containing historical data to compare to.
        key: A key in a datapoint or its telemetry dict to be tested.

    Returns:
        True if the data point is more than 'stds' standard deviations from the
        mean for data from the past hour for any telemetry key, or False
        otherwise.
    """

    for key in data:
        if anomaly_classify_by_key(stds, datum, data, data[key]) == True:
            return True

    return False
