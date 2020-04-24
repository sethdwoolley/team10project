"""A flask blueprint which adds user functionality to the Solanum system.

This blueprint calls various database functions from databaseAccess and implements them into endpoints.
It allows the front-end to:
- Add a user.
- Remove a user.

Adding users to a group is handled in group_data.py

Typical usage examples:

"""
from flask import Blueprint, request
from databaseAccess import DatabaseAccess


user_api = Blueprint('user_api', __name__)

# TODO: ASK ABOUT POSTING USERS - no way to add a user
# GET
# Retrieve user IDs up to a specified amount (if given)
# no way of doing this with current database functions
@user_api.route('/users<limit>', methods=["GET"])
def users():
    limit = request.args.get("limit")
    # TODO: PERMITTED USER ACCESS LIST, also how to retrieve a specified number of users


# Retrieve the attributes of a specified user
@user_api.route('/users/<userID>/attributes<attribute>', methods=["GET", "DELETE"])
def user_attribute(userID):
    if request.method == "GET":
        dbAccess = DatabaseAccess()
        # TODO: ONLY RETURN SPECIFIED ATTRIBUTE IF GIVEN (else all attributes?)
        attribute = request.args.get("attribute")
        return dbAccess.user_get_info()

    elif request.method == 'DELETE':
        # no way of doing this with current database functions
        # TODO: Seemingly no way of achieving this with the current databaseAccess.py
        return ''


# Edit user data
@user_api.route('/users/', methods=["POST", "DELETE"])
def edit_user():
    if request.method == "POST":
        userID = request.form.get("userID")
        username = request.form.get("username")
        credentials = request.form.get("credentials")

        dbAccess = DatabaseAccess()
        dbAccess.user_add(userID, username, credentials)
        return 'user added'

    if request.method == "DELETE":
        userID = request.form.get("userID")
        dbAccess = DatabaseAccess()
        dbAccess.user_remove(userID)
        return 'user removed successfully'




# roles were never implemented. Weren't deemed necessary.
# TODO: speak to Seth regarding role access from groups, Oli also wanted for this
# Retrieve all users of a specified role
@user_api.route('/roles/users<role>', methods=["GET"])
def user_roles(role):
    return ''


# POST
# This will create a new role
@user_api.route('/roles', methods=["GET"])
def new_roles():
    return ''



