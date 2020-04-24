"""Performs first time setup of SQL and mongo databases

Running this file will execute all the SQL and mongo commands
needed to setup both database instances. It assumes that:
1) There is an SQL instance running on localhost port 3306
2) There is an SQL user "dbsetup" with password "passwordSetup"
   that has full root permissions
3) There is a mongod instance running on localhost port 27017

"""

import mysql.connector
import pymongo
import configparser

def setup():
    """
    Main program body

    Constants:
        SQL_SERVER:     SQL server information
        PLANTS_DB:      SQL plants database information
        MONGO_SERVER:   Mongo server information

    Variables:
        sqlcursor:      SQL Server cursor
        plantscursor:   SQL plants database cursor (instantiated after SQL_SERVER is checked)
        mongo_client:   Pymongo "mongoclient" object

    Flags:
        user_not_present:     Defines if sql dbaccess user is present at program start
        database_not_present: Defines if sql plants database is present at program
                              start
    """

    serverinfo = configparser.ConfigParser()
    serverinfo.read("dbinfo.ini")

    """
    SERVERS
    """
    MONGO_SERVER = {"host":serverinfo["MONGO"]["host"],
                    "port":int(serverinfo["MONGO"]["port"])}

    SQL_SERVER = mysql.connector.connect(
        host=serverinfo["SQLSETUP"]["host"],
        user=serverinfo["SQLSETUP"]["user"],
        password=serverinfo["SQLSETUP"]["password"]
    )

    print("""
    ######## SQL ########
    """)

    sqlcursor = SQL_SERVER.cursor()

    user_not_present = False
    database_not_present = False

    # Check for dbaccess user, create if needed
    sqlcursor.execute("SELECT user FROM mysql.user WHERE user='dbaccess'")

    if len(sqlcursor.fetchall()) != 1:
        user_not_present = True
        print("dbaccess User not present, creating")
        sqlcursor.execute("CREATE USER '%s'@'%%' IDENTIFIED BY '%s'" %
                          (serverinfo["SQLACCESS"]["user"],
                           serverinfo["SQLACCESS"]["password"]))
        print(sqlcursor)
        print("dbaccess User created")

    # Check for plants database, create if needed
    sqlcursor.execute("SHOW DATABASES LIKE 'plants'")

    if len(sqlcursor.fetchall()) != 1:
        database_not_present = True
        print("plants database not present, creating")
        sqlcursor.execute("CREATE DATABASE %s" % serverinfo["SQLACCESS"]["database"])
        print("plants database created")

    if user_not_present or database_not_present:
        print("dbaccess does not have permissions, adding")

        sqlcursor.execute("GRANT INSERT,UPDATE,DELETE,SELECT ON %s.* TO '%s'@'%%'" %
                          (serverinfo["SQLACCESS"]["database"],
                           serverinfo["SQLACCESS"]["user"]))
        print("Added dbaccess permissions")

    # Instantiate plants database cursor
    PLANTS_DB = mysql.connector.connect(
        host=serverinfo["SQLSETUP"]["host"],
        user=serverinfo["SQLSETUP"]["user"],
        password=serverinfo["SQLSETUP"]["password"],
        database=serverinfo["SQLSETUP"]["database"]
    )

    plantscursor = PLANTS_DB.cursor()

    # List of all tables needed
    tables = ["User", "GroupNoConflict", "Permission", "Device"]
    creators = {"User":"""CREATE TABLE User (

                          userID int NOT NULL AUTO_INCREMENT,
                          userName VARCHAR(32) NOT NULL, 
                          userCredentials VARCHAR(128) NOT NULL,

                          PRIMARY KEY (userID)
                          )""",

                "GroupNoConflict":"""CREATE TABLE GroupNoConflict (

                                     groupID int AUTO_INCREMENT,
                                     parentID int,
                                     groupName VARCHAR(32) NOT NULL,

                                     PRIMARY KEY (groupID),
                                     FOREIGN KEY (parentID) REFERENCES GroupNoConflict(groupID)
                                     )""",

                "Permission":"""CREATE TABLE Permission (

                                permissionID int AUTO_INCREMENT,
                                userID int,
                                groupID int,
                                type ENUM('admin','user'),

                                PRIMARY KEY (permissionID),
                                FOREIGN KEY (userID) REFERENCES User(userID),
                                FOREIGN KEY (groupID) REFERENCES GroupNoConflict(groupID)
                                )""",

                "Device":"""CREATE TABLE Device (

                            deviceID CHAR(32),
                            groupID int NOT NULL,
                            ipAddress VARCHAR(16),
                            authToken CHAR(32),
                            deviceName VARCHAR(32),

                            PRIMARY KEY (deviceID),
                            FOREIGN KEY (groupID) REFERENCES GroupNoConflict(groupID)
                            )"""
               }

    for table in tables:
        plantscursor.execute("SHOW TABLES LIKE '%s'" % table)

        if len(plantscursor.fetchall()) != 1:
            print("Table %s not present, creating" % table)
            plantscursor.execute(creators[table])
            print("Table %s created" % table)

        else:
            print("Table %s present" % table)

    print("""
    ####### MONGO #######
    """)

    print("Connecting to mongodb client at host %s port %s" %
          (MONGO_SERVER["host"], MONGO_SERVER["port"]))

    # try:
    mongo_client = pymongo.MongoClient(MONGO_SERVER["host"], MONGO_SERVER["port"])
    print("Connected:", str(mongo_client.address))

    mongo_client.drop_database("devices")
    mongo_client.drop_database("users")
    mongo_client.drop_database("rules")

    mongo_databases = mongo_client.list_database_names()

    mongo_needed = ["users", "devices", "rules"]
    for database in mongo_needed:
        if database not in mongo_databases:
            print("%s database not present, creating" % database)
            db = mongo_client[database]
            newcollection = db["setupCollection"]
            #newcollection.insert_one({"init":"init"})
            print("%s database created" % database)



    print("""
    ### SETUP COMPLETE ###
    """)




if __name__ == "__main__":
    setup()
    