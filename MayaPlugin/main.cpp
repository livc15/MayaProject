#include "maya_includes.h"
#include "info.h"

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

void getTransfromData(MFnTransform& transformer)
{
	MMatrix transformationMatrix = transformer.transformation().asMatrix();
}
void OutputMeshVerticis(MFnMesh& mesh, meshData& mshData)
{
	/*if()
	mshData.meshName;

	MPointArray vts;
	mesh.getPoints(vts); 
	int nrOfVts = vts.length(); 

	for (int i = 0; i < nrOfVts; i++)
	{
		vertexData vrtData;
		vertex tempVert;

		vrtData.vertexID = i;
		vertexList.push_back(vts);

 		tempVert.vertexPos[0] = vts[i].x;
		tempVert.vertexPos[1] = vts[i].y;
		tempVert.vertexPos[2] = vts[i].z;

		mshData.vertexList.push_back(tempVert);
	}*/
}

void attributeChangedCallback(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void*clientData)
{
	if (msg & MNodeMessage::AttributeMessage::kAttributeSet)
	{
		if (plug.node().apiType() == MFn::kTransform)
		{
			MFnTransform transform(plug.node()); 

			if (transform.child(0).hasFn(MFn::kMesh))
			{
				MFnMesh mesh = transform.child(0);
				meshData mData;
				mData.meshName = mesh.absoluteName().asChar();

				getTransfromData(transform);
				OutputMeshVerticis(mesh, mData);

			}
		}
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