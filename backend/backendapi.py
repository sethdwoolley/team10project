"""Backend API endpoint for receiving, validating and forwarding data received from the hardware.

Creates an exposed endpoint to allow the wifi connected hardware to POST json data there.
The endpoint is /devices/<device>/telemetry with <devices> being the device ID.
The expected data format will be validated and reconstructed into a dictionary and sent
to the database.
"""


import datetime
from datetime import datetime
from databaseAccess import DatabaseAccess
from flask import request, jsonify, Blueprint

backend_api = Blueprint("backend_api", __name__)


@backend_api.route('/devices/<device>/telemetry', methods=['POST'])
def post_device_telemetry(device):
    """API Endpoint to receive, validate and forward POST data received from the hardware.

    The created /devices/<device>/telemetry with <devices> being the 16 Digit device ID
    will be used by the hardware to POST json data.
    The data format that is expected is:
    {
        "ts": 1587479991,
        "data": [
            {
                "name": "temperature",
                "value": 19.3
            }
        ]
    }
    Once received, it would would be validated by checking if a timestamp is included
    if not then assign the server one.
    Data list would also be validated and check if each telemetry reading includes a name
    and a value, also it makes sure that neither of those values are empty, if they are missing
    or are empty then the specific telemetry reading is not included in the final dictionary.
    The final dictionary formatted as follows:
    {
        "data":
        {
            "temperature": 19.3
        },
        "ts": 1587479991
    }
    Print statements are implemented in different parts to output the API's process to the console.

    Args:
        device: The 16 digit device ID received from the hardware when it accesses the endpoint.

    Returns:
        HTTP Status code
        200 if the POST request happened successfully.
        401 if the device_id doesn't exist in the database.
    """

    #Accessing database
    dbAccess = DatabaseAccess()

    #If device not on list return 401
    if not dbAccess.device_exists(device):
        return '401'

    #Receiving request
    tele_data = request.get_json()
    print("Request received.")

    #If no timestamp is present or is empty in the POST request assign epoch server time
    if not "ts" in tele_data or not tele_data["ts"]:

        tele_timestamp = int(datetime.utcnow().timestamp())
        print("No timestamp received, assigning server: " + str(tele_timestamp))

    else:
        tele_timestamp = tele_data["ts"]


    #Validating incoming data
    #If no name or reading detected or are empty drop the current reading and move to the next
    #Else store in data_list and data_dict as dictionary
    data_list = []
    data_dict = {}
    unvalid_data_no = 0
    data_length = len(tele_data["data"])

    for i in range(data_length):

        if not "name" in tele_data["data"][i] or not "value" in tele_data["data"][i]:

            if "name" in tele_data["data"][i] or not "value" in tele_data["data"][i]:
                print(str(tele_data["data"][i]["name"]) + " contains no value and will not be included.")

            elif "value" in tele_data["data"][i]:
                print("Value with no name attached detected and will not be included.")
            
            unvalid_data_no = unvalid_data_no + 1
            continue

        else:

            tele_name = tele_data["data"][i]["name"]
            tele_value = tele_data["data"][i]["value"]

            if not tele_value or not tele_name:

                if not tele_value and tele_name:
                    print(str(tele_name) + " contains empty value and will not be included.")

                elif tele_value and not tele_name:
                    print("Value with empty name attached detected and will not be included.")

                unvalid_data_no = unvalid_data_no + 1
                continue

            else:
                data_list.append(tele_data["data"][i])
                data_dict[tele_data["data"][i]["name"]] = tele_data["data"][i]["value"]

    #Formats the data for the database and anomaly detection
    tele_valid_data = {"ts" : tele_timestamp, "data" : data_dict}

    #Echoes validated data received
    valid_data_length = data_length - unvalid_data_no
    print("Data received from Device: " + str(device))
    print("Time: " + str(tele_timestamp))
    print("Containing Data:")

    for i in range(valid_data_length):
        print("Name: " + str(data_list[i]["name"]) + " with value: " + str(data_list[i]["value"]))
    

    #Database
    print("Sending to database.")
    dbAccess.telemetry_add(device, tele_valid_data)

    return '200'
