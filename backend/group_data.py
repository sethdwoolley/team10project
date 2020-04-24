"""A flask blueprint which adds group functionality to the Solanum system.

This blueprint calls various database functions from databaseAccess and implements them into endpoints.
It allows the front-end to:
- Add new groups.
- Change group name of a specified group.
- Add a specified user to a specified group.
- Add parent groups to a specified group.
- Return attributes for a specified group.
- Return all devices within a specified group.
- Return all users within a specified group.
- Return parent groups of a specified group

Typical usage example:

"""



from flask import Blueprint, request
from databaseAccess import DatabaseAccess


group_api = Blueprint('group_api', __name__)


# GET: Retrieves group IDs as well as their attributes up to a specified limit (if provided)
# POST: Adds a new group
@group_api.route('/groups', methods=["GET", "POST"])
def group_data():
    """
    Retrieves group data, and allows for addition of new groups.

    GET: should retrieve all groupID's a user is in and their names but due to constraints with timing this was not possible.
    POST: adds a new group ot the database. By calling the group add function of the DatabaseAccess() class.

    Form:
        groupName: a string containing the name of the group to be added.

    :return:
        'group added successfully.' message if a groupName was present in the form data.

        'no group name, no group created.' message if no groupName present in form data.
        This allows the code to fail silently rather than returning an error message to disrupt the server/front-end.

    """
    dbAccess = DatabaseAccess()
    if request.method == "GET":
        # this cannot be implemented with current DB code
        # TODO: add a default val for limit that ensures there can be a missing limit param

        return ''

    elif request.method == "POST":
        # TODO: add a format check for groupIDs, otherwise reject
        groupName = request.form.get('groupName')
        if request.form.get("groupName"):
            dbAccess.group_add(groupName)
            return 'group added successfully, '
        else:
            return 'no group name, no group created.'


# Retrieves attributes for a specific group
@group_api.route('/groups/', methods=["GET", "POST", "DELETE"])
def groups():
    """
    Handles the retrieval, group name changes and deletion of groups.

    GET: returns all relevant data about the group specified in the groupID form data.
    POST: allows for the user to change the name of the group specified in the groupID form data.
    DELETE: allows for the deletion of the group specified in the groupID form data.

    Ideally a check for if group exists would be present to give another fail safe to the functions.

    Form:
        groupID: an integer containing the ID of a requested group

    :return:
        'no groupID present, no functions will be carried out.' will be returned if a groupID is missing.
        'data to be returned' will be returned if the GET function is called an a groupID is present.
        'group name changed' will be returned if the POST function is called an a groupName/groupID is provided.
        'no group name given. no changes made.' will be returned if the POST function is called, a groupID is provided and a groupName is missing.
        'group removed successfully.' will be returned if DELETE function is called with a groupID provided.
    """
    groupID = int(request.form.get('groupID'))
    if not request.form.get('groupID'):
        return 'no groupID present, no functions will be carried out.'
    if request.method == "GET":
        dbAccess = DatabaseAccess()
        groupName = dbAccess.group_get_name(groupID)
        groupUser = dbAccess.group_get_users(groupID)
        groupParent = dbAccess.group_get_parent(groupID)
        # No function actually exists to set children. This function is useless.
        # groupChildren= dbAccess.group_get_children(groupID)
        groupDevices = dbAccess.group_get_devices(groupID)
        groupAttributes = [groupName, groupUser, groupParent, groupDevices]
        print(groupAttributes)
        return 'data to be returned'

    elif request.method == "POST":
        if request.form.get("groupName"):
            dbAccess = DatabaseAccess()
            dbAccess.group_set_name(groupID, request.form.get("groupName"))
            return 'group name changed'
        else:
            return 'no group name given. no changes made.'

    elif request.method == "DELETE":
        dbAccess = DatabaseAccess()
        dbAccess.group_remove(groupID)
        return 'group removed successfully.'



# Returns all devices within a specific group
@group_api.route('/groups/<groupID>/devices', methods=["GET"])
def groups_devices(groupID):
    """
    Returns all deviceID's stored in a specified group.

    GET: returns all deviceIDs within a specified group. DOES NOT return device data as the supporting database function does not exist.

    :param
        groupID: an integer containing the ID of a requested group

    :return:
        'no groupID present, no function will be carried out' will be returned if the groupID is missing
        retrievedData will be returned if the group_get_devices function worked. This data is a dict of DeviceID's
    """
    groupID = int(groupID)
    if request.method == "GET":
        dbAccess = DatabaseAccess()
        retrievedData = dbAccess.group_get_devices(groupID)
        print("\n", retrievedData, "\n")
        retrievedData = {i: retrievedData[i] for i in range(0, len(retrievedData))}
        return retrievedData


# Returns all users in a specific group
@group_api.route('/groups/<groupID>/users', methods=["GET", "POST"])
def groups_users(groupID):
    """
    Allows for retrieval and addition of users in a specified group.

    GET: returns all users of a specified group.
    POST: adds a user of a specified userID to a group of a specifiedID

    Form:
        userID: an integer value corresponding to a user account

    :param
        groupID: an integer value corresponding to a group

    :return:
        'no groupID present, no functions will be carried out.' will be returned if no groupID is entered.
        'no userID present, no function will be carried out.' will be returned if no userID is contained in the form data.
         retrievedData will be returned if the GET function is called with a groupID present.
        This is a dict containing users of the specified group
        'permission added' will be returned if the POST function is called with acceptable data.
    """
    groupID = int(groupID)
    if request.method == "GET":
        dbAccess = DatabaseAccess()
        retrievedData = dbAccess.group_get_users(groupID)
        print(retrievedData)
        retrievedData = {i: retrievedData[i] for i in range(0, len(retrievedData))}
        return retrievedData

    elif request.method == "POST":
        if not request.form.get('userID'):
            return 'no userID present, no function will be carried out.'
        userID = int(request.form.get("userID"))
        dbAccess = DatabaseAccess()
        dbAccess.permission_add(userID, groupID)
        return 'permission added'


# Retrieve all groups within a selected group, parents and children
# Groups are nestable to create a tree structure
# Post will add a parent group to the requested group.
# Function missing here, so broken child link
@group_api.route('/groups/<groupID>/groups', methods=["GET", "POST"])
def nested_groups(groupID):
    """
    Allows for a retrieval and addition of parents of a group.
    Ideally there would be a check to verify that the groups exist.

    GET: returns all parent groupIDs of a specified group.
    POST: adds a parent group to a specified group.

    :param
        groupID: an integer value containing an ID of a group

    :form
        parentID: an integer value containing an ID of a group to be added as a parent.

    :return:
        retrievedData will be returned if the GET function is called with a valid groupID.
        'group added successfully' will be returned if the POST function is called with a valid groupID and parentID.
    """
    groupID = int(groupID)
    if request.method == "GET":
        dbAccess = DatabaseAccess()
        retrievedData = dbAccess.group_get_parent(groupID)
        retrievedData = {i: retrievedData[i] for i in range(0, len(retrievedData))}
        print("\n", retrievedData, "\n")
        return retrievedData

    if request.method == "POST":
        parentGroup = int(request.form.get("parentID"))
        dbAccess = DatabaseAccess()
        dbAccess.group_set_parent(groupID, parentGroup)
        return 'group added successfully'


