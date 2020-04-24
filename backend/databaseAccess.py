# pylint: disable=C0330,R0904
"""Defines a class to allow easy access to database functions

Simplifies mongodb and SQL functions into python functions so
other group members don't have to implement the code themselves.

    Typical usage example:

    db = DatabaseAccess()
    db.user_add("userXYZ", "Seth Woolley", "xeIWI19EFxe")
    db.user_get_info("userXYZ")
"""

import configparser
import mysql.connector
import pymongo

class DatabaseAccess():
    """ Class to allow easy access to database functions.

    Simplifies mongodb and SQL functions into python functions so
    other group members don't have to implement the code themselves.
    """

    def __init__(self):

        serverinfo = configparser.ConfigParser()
        serverinfo.read("dbinfo.ini")

        self._mongo_object = pymongo.MongoClient(serverinfo["MONGO"]["host"],
                                                 int(serverinfo["MONGO"]["port"]))

        self._mongo_devices = self._mongo_object["devices"]

        self._sql_object = mysql.connector.connect(host=serverinfo["SQLACCESS"]["host"],
                                                   user=serverinfo["SQLACCESS"]["user"],
                                                   password=serverinfo["SQLACCESS"]["password"],
                                                   database=serverinfo["SQLACCESS"]["database"])

        self._sql_cursor = self._sql_object.cursor()

    # Creators/removers
    def user_add(self, user_id, name, credentials):
        self._sql_cursor.execute("""INSERT INTO User (
                                    userName, userCredentials)
                                    VALUES ('%s', '%s')""" % (name, credentials))

        # print(self._sql_cursor.rowcount)
        self._sql_object.commit()

        return self._sql_cursor.lastrowid

    def user_remove(self, user_id):
        self._sql_cursor.execute("""
                                 DELETE FROM User
                                 WHERE userID = '%s';
                                 """ % user_id)

        self._sql_object.commit()

        return self._sql_cursor.rowcount

    def group_add(self, group_id):
        self._sql_cursor.execute("""
                                 INSERT INTO GroupNoConflict (groupName)
                                 VALUES ('%s')
                                 """ % group_id)

        self._sql_object.commit()

        return self._sql_cursor.lastrowid

    def group_set_name(self, group_id, group_name):
        self._sql_cursor.execute("""
                                 UPDATE GroupNoConflict
                                 SET groupName = '%s'
                                 WHERE groupID = %s
                                 """ % (group_name, group_id))

    def group_set_parent(self, group_id, parent_id):
        self._sql_cursor.execute("""
                                 UPDATE GroupNoConflict
                                 SET parentID = %s
                                 WHERE groupID = %s
                                 """ % (parent_id, group_id))
        self._sql_object.commit()

        return True

    def group_remove(self, group_id):
        self._sql_cursor.execute("""
                                 UPDATE GroupNoConflict
                                 SET groupParent = NULL
                                 WHERE groupParent = '%s'
                                 """ % group_id)

        self._sql_cursor.execute("""
                                 DELETE FROM GroupNoConflict
                                 WHERE groupID = '%s'
                                 """ % group_id)

        self._sql_object.commit()

        return self._sql_cursor.rowcount()

    def permission_add(self, user_id, group_id):
        self._sql_cursor.execute("""
                                 INSERT INTO Permission (userID,groupID)
                                 VALUES ('%s','%s')
                                 """ % (user_id, group_id))

        self._sql_object.commit()

        return self._sql_cursor.lastrowid

    def permission_remove(self, user_id, group_id):
        self._sql_cursor.execute("""
                                 DELETE FROM Permission
                                 WHERE userID = '%s', groupID = '%s'
                                 """, (user_id, group_id))

        self._sql_object.commit()

        return self._sql_cursor.rowcount()

    def device_add(self, device_id, group_id, ip, token):

        self._sql_cursor.execute("""
                                 INSERT INTO Device (deviceID,groupID,IPAddress,authToken)
                                 VALUES ('%s', '%s', '%s', '%s')
                                 """ % (device_id, group_id, ip, token))

        self._sql_object.commit()

        mongo_addition = self._mongo_devices[device_id]
        mongo_addition.insert_one({"init":"init"})

        return device_id

    def device_remove(self, device_id): # TODO
        self._sql_cursor.execute("""
                                 DELETE FROM Device
                                 WHERE deviceID = '%s'
                                 """ % device_id)

        self._sql_object.commit()

        self._mongo_object["devices"][device_id].drop();

        return self._sql_cursor.rowcount

    def telemetry_add(self, device_id, telemetry_data):
        addition = self._mongo_devices[device_id].insert_one(telemetry_data)
        return addition.acknowledged

    def rule_add(self, group_id, rule_data):
        return True

    def rule_remove(self, group_id, rule_id):
        return True

    # Queries
    def user_get_info(self, user_id):
        self._sql_cursor.execute("""
                                 SELECT userName
                                 FROM User
                                 WHERE userID = '%s'
                                 """ % user_id)

        return self._sql_cursor.fetchall()

    def user_exists(self, user_id):
        self._sql_cursor.execute("""
                                 SELECT userID
                                 FROM User
                                 WHERE userID = '%s'
                                 """ % user_id)

        return len(self._sql_cursor.fetchall()) > 0

    def user_get_credentials(self, user_id):
        self._sql_cursor.execute("""
                                 SELECT userCredentials
                                 FROM User
                                 WHERE userID = '%s'
                                 """ % user_id)
        return self._sql_cursor.fetchall()

    def user_get_permitted(self, user_id, limit=None): # TODO limit
        self._sql_cursor.execute("""
                                 SELECT groupID
                                 FROM Permission
                                 WHERE userID = '%s'
                                 """ % user_id)
        return self._sql_cursor.fetchall()

    def group_get_name(self, group_id):
        self._sql_cursor.execute("""
                                 SELECT groupName
                                 FROM GroupNoConflict
                                 WHERE groupID = %d
                                 """ % group_id)

        return self._sql_cursor.fetchall()

    def group_get_users(self, group_id, limit=None): # TODO limit
        self._sql_cursor.execute("""
                                 SELECT userID
                                 FROM Permission
                                 WHERE groupID = %d
                                 """ % group_id)
        return self._sql_cursor.fetchall()

    def group_get_parent(self, group_id):
        self._sql_cursor.execute("""
                                 SELECT parentID
                                 FROM GroupNoConflict
                                 WHERE groupID = %d
                                 """ % group_id)

        return self._sql_cursor.fetchall()

    def group_get_children(self, group_id, limit=None): # TODO limit
        self._sql_cursor.execute("""
                                 SELECT groupID
                                 FROM GroupNoConflict
                                 WHERE parentID = %d
                                 """ % group_id)

        return self._sql_cursor.fetchall()

    def group_get_devices(self, group_id, limit=None): # TODO limit
        self._sql_cursor.execute("""
                                 SELECT deviceID
                                 FROM Device
                                 WHERE groupID = %d
                                 """ % group_id)

        return self._sql_cursor.fetchall()


    def device_exists(self, device_id):
        self._sql_cursor.execute("""
                                 SELECT deviceID
                                 FROM Device
                                 WHERE deviceID = '%s'
                                 """ % device_id)

        return len(self._sql_cursor.fetchall()) != 0

    def telemetry_get_latest(self, device_id):
        return self._mongo_devices[device_id].find().sort("ts", -1).limit(1)[0]

    def telemetry_get(self, device_id, limit):
        if limit:
            return list(self._mongo_devices[device_id].find().sort("ts",-1).limit(limit))
        else:
            return list(self._mongo_devices[device_id].find().sort("ts",-1, pymongo.DESCENDING))

    def print_sql_database(self):
        for table in ["User", "GroupNoConflict", "Permission", "Device"]:
            self._sql_cursor.execute("SELECT * FROM %s" % table)
            print(table, ":", self._sql_cursor.fetchall())
