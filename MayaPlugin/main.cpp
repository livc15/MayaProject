#include "maya_includes.h"

MCallbackIdArray myCallbackArray;

void nameChangeCallback(MObject& node, const MString &string, void* clientData)
{
	if (node.hasFn(MFn::kMesh))
	{
		MFnMesh mesh(node);
		MString msg(mesh.name());
		MGlobal::displayInfo(msg);
	}
	else if (node.hasFn(MFn::kTransform))
	{
		MFnTransform transform(node);
		MString msg(transform.name());
		MGlobal::displayInfo(msg);
	}
}
void nodeCreationCallback(MObject& node, void* clientData)
{
	MStatus res;

	if (node.apiType() == MFn::kMesh)
	{
		MCallbackId meshNameChangeID = MNodeMessage::addNameChangedCallback(
			node,
			nameChangeCallback,
			NULL,
			&res);

		if (res == MS::kSuccess)
		{
			if (myCallbackArray.append(meshNameChangeID) == MS::kSuccess);
		}
	}
	else if (node.apiType() == MFn::kTransform)
	{
		MCallbackId transformNameChangeID = MNodeMessage::addNameChangedCallback(
			node,
			nameChangeCallback,
			NULL,
			&res);

		if (res == MS::kSuccess)
		{
			if (myCallbackArray.append(transformNameChangeID) == MS::kSuccess);
		}
	}

}

void attributeChangedCallback(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void*clientData)
{
	MStatus res;

	if (msg & MNodeMessage::AttributeMessage::kAttributeSet)
	{
		
	}

}

EXPORT MStatus initializePlugin(MObject obj) 
{
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
	if (MFAIL(res)) 
	{
		CHECK_MSTATUS(res);
		return res;
	}
	else {
		MGlobal::displayInfo("Maya plugin loaded!");
	}

	MCallbackId nodeAddedId = MDGMessage::addNodeAddedCallback(
		nodeCreationCallback,
		kDefaultNodeType,
		NULL,
		&res);

	if (res == MS::kSuccess)
	{
		if (myCallbackArray.append(nodeAddedId) == MS::kSuccess);
	}

	MCallbackId AttributeChangedID = MNodeMessage::addAttributeChangedCallback(
		obj,
		attributeChangedCallback,
		NULL,
		&res);

	if (res == MS::kSuccess)
	{
		if (myCallbackArray.append(AttributeChangedID) == MS::kSuccess);
	}



}

EXPORT MStatus uninitializePlugin(MObject obj) 
{
	MFnPlugin plugin(obj);

	MMessage::removeCallbacks(myCallbackArray);
	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}