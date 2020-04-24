"""
File used to test database
"""

import sys
import configparser
import mysql.connector
import pymongo
from databaseAccess import DatabaseAccess
import databaseSetup

def main():
    """
    File used to test database
    """

    serverinfo = configparser.ConfigParser()
    serverinfo.read("dbinfo.ini")

    answer = input("WARNING: THIS WILL WIPE THE DATABASE. DO YOU WANT TO CONTINUE? (Y/N): ")
    if answer.upper() != ("Y"):
        sys.exit()

    # delete sql database
    SQL_SERVER = mysql.connector.connect(
        host=serverinfo["SQLSETUP"]["host"],
        user=serverinfo["SQLSETUP"]["user"],
        password=serverinfo["SQLSETUP"]["password"]
    )

    sqlcursor = SQL_SERVER.cursor()

    try:
        sqlcursor.execute("DROP DATABASE plants")
    except:
        print("Error deleting plants database:", sys.exc_info()[0])

    # delete mongo database
    MONGO_SERVER = {"host":serverinfo["MONGO"]["host"],
                    "port":int(serverinfo["MONGO"]["port"])}

    mongo_client = pymongo.MongoClient(MONGO_SERVER["host"], MONGO_SERVER["port"])

    mongo_client.drop_database("devices")
    mongo_client.drop_database("users")
    mongo_client.drop_database("rules")

    # Setup
    databaseSetup.setup()
    database = DatabaseAccess()

    print("""
    #### TESTING ####
    """)

    # Add user and groups
    user = database.user_add(None, "Seth Woolley", "testCredentials")
    parent_group = database.group_add("ParentGroup")
    child_group = database.group_add("ChildGroup")

    print("Parent group: %s\nChild group: %s\n" % (parent_group, child_group))

    # Set group parent and permission, add device
    database.group_set_parent(child_group, parent_group)
    database.permission_add(user, child_group)
    device = database.device_add("devicexxxxxxxxxxxxxxxxxxxxxxxxxx", child_group,
                                 "192.168.1.1", "authtokenxxxxxxxxxxxxxxxxxxxxxxx")

    # Add device telemetry
    database.telemetry_add(device, {"timestamp":1587126650, "data":{"x":100, "y":100}})

    print() # USER CHECKS

    print("Get user info: ", database.user_get_info(user))
    print("Check existing user exists:", database.user_exists(user))
    print("Check nonexisting user doesn't exist:", database.user_exists(240))
    print("Get user credentials: ", database.user_get_credentials(user))
    print("User get permissions: ", database.user_get_permitted(user))

    print() # GROUP CHECKS

    print("Get parent group name:", database.group_get_name(parent_group))
    print("Child group get users:", database.group_get_users(child_group))
    print("Child group check parent: ", database.group_get_parent(child_group))
    print("Parent group check children: ", database.group_get_children(parent_group))
    print("Child group check devices: ", database.group_get_devices(child_group))

    print() # DEVICE CHECKS

    print("Check existing device exists:", database.device_exists(device))
    print("Check nonexisting device doesn't exist:", database.device_exists("devicefakexxxxxxxxxxxxxxxxxxxxxx"))

    print("\nPrint whole SQL database:\n")
    database.print_sql_database()

    print("""
    ### MONGO TESTING ###
    """)

    print("All mongo databases:", mongo_client.list_database_names())
    device_database = mongo_client["devices"]
    print("Find latest telemetry:", database.telemetry_get_latest("devicexxxxxxxxxxxxxxxxxxxxxxxxxx"))
    print("Deleting device %s" % device)
    print(database.device_remove(device))
    print(device_database.list_collection_names())

if __name__ == "__main__":
    main()
