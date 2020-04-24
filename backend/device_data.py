"""A flask blueprint which adds device functionality to the Solanum system.

This blueprint calls various database functions from databaseAccess and implements them into endpoints.
It allows the front-end to:
- Add a new device.
- Return telemetry data from a specified device. Can return a specified number of results.
- Return the single latest piece of telemetry data from a specified device.
- Return aggregated data from a specified device for a specified number of data points
- Remove a device from the system.

Typical usage example:
-
"""

from flask import Blueprint, request
from databaseAccess import DatabaseAccess
import analytics


device_api = Blueprint('device_api', __name__)


# GET
# Retrieves the IDs of all devices (up to a specified limit, if given)
# cannot implement the GET function as a related database function does not exist
@device_api.route('/devices', methods=["POST", "GET"])
def device_data():
    """
    Retrieves device data, and allows for addition of new devices.

    GET: should retrieve all deviceID's from groups a user is in and their names but due to constraints with timing this was not possible.
    POST: adds a new device to the database. By calling the device add function of the DatabaseAccess() class.

    Form:
        deviceID: a string containing the name of the device to be added.
        groupID: an integer containing the ID of the group the device is to be bound to.
        id: the IP address of the device on the network.
        token: the authToken given to the device to verify it. <- JSON auth wasn't implemented.

    :return:
        'an item of data is missing' message if an item of data is missing from the form.
        'device already present. refused addition.' message if a device of the specified ID already exists.
        This allows the code to fail silently rather than returning an error message to disrupt the server/front-end.
        'device added successfully.' message if all data is present and correct in the form data.
    """
    dbAccess = DatabaseAccess()
    if request.method == "GET":
        # TODO; find out again if return limit possible
        limit = request.args.get("limit")

    elif request.method == "POST":
        deviceID = request.form.get("deviceID")
        groupID = int(request.form.get("groupID"))
        ip = request.form.get("ip")
        token = request.form.get("token")
        if (not deviceID) or (not groupID) or (not ip) or (not token):
            return 'an item of data is missing'
        if dbAccess.device_exists(deviceID):
            return 'device already present. refused addition.'
        else:
            dbAccess.device_add(deviceID, groupID, ip, token)
            return 'device added successfully.'


# Retrieves telemetry data for the specified device
#TODO: check if device exists first
@device_api.route('/devices/<deviceID>/telemetry/', methods=["GET"])
def device_telemetry(deviceID):
    """
    Retrieves telemetry data up to a specified limit.

    GET: will query the PyMongo container of the relevant deviceID and return the specified number of datapoints.

    Form:
        limit: an integer value specifying how many datapoints to return.

    :param
        deviceID: a string containing deviceID.

    :return
        'no limit present, no functions will be carried out.' is displayed if limit is missing from form data.
        'device does not exist. no data to return.' is displayed if the device with the requested ID does not exist.
        telemetryDict is a dictionary of dictionaries containing requested data.
    """
    if not request.form.get('limit'):
        return 'no limit present, no functions will be carried out.'

    limit = int(request.form.get("limit"))
    dbAccess = DatabaseAccess()
    if not dbAccess.device_exists(deviceID):
        return 'device does not exist. no data to return.'

    returnedFromDB = dbAccess.telemetry_get(deviceID, limit)

    for telemetryReadings in returnedFromDB:
        telemetryReadings.update((key, deviceID) for key, value in telemetryReadings.items() if key == "_id")

    telemetryDict = {i: returnedFromDB[i] for i in range(0, len(returnedFromDB))}

    print(telemetryDict)
    return telemetryDict


# Retrieves only the LATEST telemetry points for the specified device (from the database)
#TODO: check if device exists first
@device_api.route('/devices/<deviceID>/telemetry/latest', methods=["GET"])
def latest_device_telemetry(deviceID):
    """
    Retrieves latest telemetry data for specified device.

    GET: will query the PyMongo container of the relevant deviceID and return the latest datapoints.

    :param
        deviceID: a string containing deviceID.

    :return
        'device does not exist. no data to return.' is displayed if the device with the requested ID does not exist.
        returnedFromDB is a dict containing the latest datapoint.
    """
    dbAccess = DatabaseAccess()
    if not dbAccess.device_exists(deviceID):
        return 'device does not exist. no data to return.'
    returnedFromDB = dbAccess.telemetry_get_latest(deviceID)
    returnedFromDB['_id'] = deviceID
    print("\n",returnedFromDB, "\n")
    return returnedFromDB


# Carries out data aggregation.
@device_api.route('/devices/<deviceID>/telemetry/aggregate', methods=["GET"])
def aggregate_device_telemetry(deviceID):
    """
    returns an aggregate of a specified number of datapoints from a specified deviceID

    Form:
        dataPeriod: an integer containing the period (in seconds) that the aggregation is to carry out on

    :param
        deviceID: a string containing the ID of a device

    :return:
        'no dataPeriod present, no functions will be carried out.' will be returned if dataPeriod is missing from the form.
        'device does not exist. no data to return.' will be returned if the device does not exist in the DB.
        aggregatedDict: a dictionary of aggregated data.
    """

    dbAccess = DatabaseAccess()

    if not request.form.get('dataPeriod'):
        return 'no dataPeriod present, no functions will be carried out.'

    period = int(request.form.get("dataPeriod"))
    # floored division allows us to work out how many data points to retrieve
    flooredPeriod = period // 5

    if not dbAccess.device_exists(deviceID):
        return 'device does not exist. no data to return.'

    returnedFromDB = dbAccess.telemetry_get(deviceID, flooredPeriod)
    # Data is descending from db, not ascending. needs fixing.
    #print(returnedFromDB,"\n\n")
    aggregatedData = analytics.aggregate(returnedFromDB)
    for dict in aggregatedData:
        dict["_id"] = deviceID
    #print(aggregatedData)
    # cannot be returned in a list
    aggregatedDict = {i: aggregatedData[i] for i in range(0, len(aggregatedData))}
    print(aggregatedDict)
    # this data is slightly incorrect
    return aggregatedDict

# Carries out data argget.
@device_api.route('/devices/<deviceID>/telemetry/get', methods=["GET"])
def get_device_telemetry(deviceID):
    dbAccess = DatabaseAccess()
    period = int(request.form.get("dataPeriod"))

    func = request.form.get("function")
    key = request.form.get("key")
    count = int(request.form.get("count"))

    if (not period) or (not func) or (not key) or (not count):
        return 'an item of data is missing'

    if not dbAccess.device_exists(deviceID):
        return 'device does not exist. no data to return.'

    flooredPeriod = period // 5
    returnedFromDB = dbAccess.telemetry_get(deviceID, flooredPeriod)
    getData = analytics.get(func, returnedFromDB, key, count)
    print(getData)
    return str(getData)

# Carries out data argget.
@device_api.route('/devices/<deviceID>/telemetry/argget', methods=["GET"])
def argget_device_telemetry(deviceID):
    dbAccess = DatabaseAccess()
    period = int(request.form.get("dataPeriod"))

    func = request.form.get("function")
    key = request.form.get("key")
    count = int(request.form.get("count"))

    if (not period) or (not func) or (not key) or (not count):
        return 'an item of data is missing'

    if not dbAccess.device_exists(deviceID):
        return 'device does not exist. no data to return.'

    flooredPeriod = period // 5
    returnedFromDB = dbAccess.telemetry_get(deviceID, flooredPeriod)
    arggetData = analytics.argget(func, returnedFromDB, key, count)
    for dict in arggetData:
        dict["_id"] = deviceID

    arggetDict = {i: arggetData[i] for i in range(0, len(arggetData))}
    print(arggetDict)
    return arggetDict

# Retrieves attributes of specified device
# Impossible to implement as relevant database function does not exist
# TODO: ask about if an attribute param isn't provided. Return all or reject query?
@device_api.route('/devices/<device>/attributes', methods=["GET", "DELETE"])
def device_attribute(devic):
    if request.method == "GET":
        attr = request.args.get("attribute")
        return ''
    if request.method == "DELETE":
        attr = request.args.get("attribute")
        return ''


# POST
# This will change/add attributes to specified device
# Delete removes a device from the system
@device_api.route('/devices/<deviceID>', methods=["POST", "DELETE"])
def device_attribute_data(deviceID):
    """
    Allows for removal of devices from the database given a specified deviceID

    POST: was supposed to allow for editing and addition of attributes to a device. However no relevant database
    functions have been implemented

    DELETE: allows for removal of a device from the system
    :param
        deviceID: a string containing deviceID

    :return:
        'device does not exist. no device to remove from system.' will be returned if there is no database entry
        for requested deviceID
        'device removed successfully' will be returned if device is removed successfully.
    """
    dbAccess = DatabaseAccess()
    if request.method == "POST":
        return ''

    elif request.method == "DELETE":
        if not dbAccess.device_exists(deviceID):
            return 'device does not exist. no device to remove from system.'
        dbAccess.device_remove(deviceID)
        return 'device removed successfully'

# THIS IS HANDLED BY BACK END API
# Wrtie a new telemetry reading to MongoDB



