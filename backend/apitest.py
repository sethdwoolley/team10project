"""A testing file used to test various endpoints of the solanum API.

This will call multiple endpoints, with various different payloads.
It's aiming to test that the endpoints work as expected.

"""




import requests
import logging
import time

logging.basicConfig(level=logging.DEBUG)

# Before adding a device to the system, we need to add a group
url = 'http://localhost:5000/groups'
payload = {'groupName': 'testGroup'}
response = requests.post(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Adding another group to test endpoints for adding parents and children
url = 'http://localhost:5000/groups'
payload = {'groupName': 'parentGroup'}
response = requests.post(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Testing adding a parent group
url = 'http://localhost:5000/groups/1/groups'
payload = {"parentID": 2}
response = requests.post(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Testing retrieval of parents
url = 'http://localhost:5000/groups/1/groups'
response = requests.get(url)
print("Server returned: ", response)

time.sleep(10)
# Needing to add a new device to the system
url = 'http://localhost:5000/devices'
payload = {'deviceID': 'devicexxxxxxxxxxxxxxxxxxxxxxxxxx',
            'groupID': '1',
            'ip': '192.168.1.1',
            'token': 'authtokenxxxxxxxxxxxxxxxxxxxxxxx',
            }
response = requests.post(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Adding a new user to the database
url = 'http://localhost:5000/users/'
payload = {
    'userID': None,
    'username': 'testUser',
    'credentials': 'testPass'}
response = requests.post(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Retrieving attributes about a group
url = 'http://localhost:5000/groups/'
payload = {'groupID': 1}
response = requests.get(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Adding a user to a group
url = 'http://localhost:5000/groups/1/users'
payload = {'userID': 1}
requests.post(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Retrieving just the user list of a group
url = 'http://localhost:5000/groups/1/users'
requests.get(url)
print("Server returned: ", response)

time.sleep(10)
# Retrieving the devices in a group
url = 'http://localhost:5000/groups/1/devices'
requests.get(url)
print("Server returned: ", response)

time.sleep(10)
# Set group name test
url = 'http://localhost:5000/groups/'
payload = {'groupID': 1,
           'groupName': 'nameChangeTest'}
requests.post(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# This is aiming to test the front-end, so will handle the addition of data to the telemetry DB slightly differently
# Test for adding data from a device
url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry'
payload = {"ts":1587658833,
           "data":[{"name":"temperature","value":19.3},
                   {"name":"humidity","value":36.8},
                   {"name":"lux","value":999},
                   {"name":"moisture","value":5.6}]}
response = requests.post(url, json=payload)
print("Server returned: ", response)

url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry'
payload ={"ts":1587658838,
          "data":[{"name":"temperature","value":20.9},
                  {"name":"humidity","value":36.5},
                  {"name":"lux","value":999},
                  {"name":"moisture","value":5.1}]}
response = requests.post(url, json=payload)
print("Server returned: ", response)

url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry'
payload = {"ts":1587658843,
           "data":[{"name":"temperature","value":20.8},
                   {"name":"humidity","value":36.4},
                   {"name":"lux","value":999},
                   {"name":"moisture","value":5.6}]}
response = requests.post(url, json=payload)
print("Server returned: ", response)

url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry'
payload = {"ts":1587658848,
           "data":[{"name":"temperature","value":20.8},
                   {"name":"humidity","value":36.4},
                   {"name":"lux","value":999},
                   {"name":"moisture","value":5.6}]}
response = requests.post(url, json=payload)
print("Server returned: ", response)

url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry'
payload ={"ts":1587658853,
          "data":[{"name":"temperature","value":20.7},
                  {"name":"humidity","value":36.3},
                  {"name":"lux","value":999},
                  {"name":"moisture","value":5.8}]}
response = requests.post(url, json=payload)
print("Server returned: ", response)

time.sleep(10)
# Testing retrieval of the latest piece of telemetry
url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry/latest'
response = requests.get(url)
print("Server returned: ", response)

time.sleep(10)
# Testing retrieval of telemetry up to a limit
url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry/'
payload = {"limit": "5"}
response = requests.get(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Testing aggregation of data
url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry/aggregate'
payload = {'dataPeriod': '16'}
response = requests.get(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Testing analytics get function
url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry/get'
payload = {'dataPeriod': '16',
           'function': 'np.max',
           'key': 'temperature',
           'count': 1}
response = requests.get(url, data=payload)
print("Server returned: ", response)

time.sleep(10)
# Testing analytics argget function
url = 'http://localhost:5000/devices/devicexxxxxxxxxxxxxxxxxxxxxxxxxx/telemetry/argget'
payload = {'dataPeriod': '16',
           'function': 'np.max',
           'key': 'temperature',
           'count': 1}
response = requests.get(url, data=payload)
print("Server returned: ", response)









