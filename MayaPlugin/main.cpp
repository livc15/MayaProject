#include "maya_includes.h"
#include "info.h"

MCallbackIdArray myCallbackArray;

//When the in-game scene camera changes
void renderChangeCallback(const MString &str, void *clientData)
{
	//Fetches the current viewport.
	M3dView sceneView;
	sceneView = sceneView.active3dView();
	//Fetches the current camera.
	MDagPath camDag;
	sceneView.getCamera(camDag);

	//Checks if it actually is a camera.
	if (camDag.node().apiType() == MFn::kCamera)
	{
		MFnCamera cam = camDag.node();
		//Fetches the parent of the camera which will always be a transformer
		MFnTransform camTransform = cam.parent(0);
		MMatrix mat = camTransform.transformation().asMatrix().matrix;

		MString camName = camTransform.absoluteName();
		MGlobal::displayInfo(MString("Cam:") + mat.matrix[0][0] + " " + mat.matrix[0][1] + " " + mat.matrix[0][2] + " " + mat.matrix[0][3]);
		MGlobal::displayInfo(MString("Cam:") + mat.matrix[1][0] + " " + mat.matrix[1][1] + " " + mat.matrix[1][2] + " " + mat.matrix[1][3]);
		MGlobal::displayInfo(MString("Cam:") + mat.matrix[2][0] + " " + mat.matrix[2][1] + " " + mat.matrix[2][2] + " " + mat.matrix[2][3]);
		MGlobal::displayInfo(MString("Cam:") + mat.matrix[3][0] + " " + mat.matrix[3][1] + " " + mat.matrix[3][2] + " " + mat.matrix[3][3]);

		double ARO = cam.aspectRatio();
		double HFOV = cam.horizontalFieldOfView();
		double VFOV = cam.verticalFieldOfView();
		double nPlane = cam.nearClippingPlane();
		double fPlane = cam.farClippingPlane();

		MGlobal::displayInfo(MString("Cam:") + ARO);
		MGlobal::displayInfo(MString("Cam:") + HFOV);
		MGlobal::displayInfo(MString("Cam:") + VFOV);
		MGlobal::displayInfo(MString("Cam:") + nPlane);
		MGlobal::displayInfo(MString("Cam:") + fPlane);
	}
}

void nameChangeCallback(MObject& node, const MString &string, void* clientData)
{
	// Fetches the name of the object that has been changed.
	// Problem: We do not know the name before the change. perhaps -> NodeRename(MObject &node, const MString &LastName, void* clientData) will work?
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
	//Function gets called everytime we select an object and a temp Node is created?
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
	MString dispScale;
	MString dispTrans;
	MString dispQuat;

	//Scale
	double scale[3] = { 1, 1, 1 };
	transformer.getScale(scale);

	dispScale += "Scale: ";
	dispScale += "X: ";
	dispScale += scale[0];
	dispScale += "Y: ";
	dispScale += scale[1];
	dispScale += "Z: ";
	dispScale += scale[2];

	//Translation
	MVector translation = transformer.getTranslation(MSpace::kObject);

	dispTrans += "Translation: ";
	dispTrans += "X: ";
	dispTrans += translation.x;
	dispTrans += "Y: ";
	dispTrans += translation.y;
	dispTrans += "Z: ";
	dispTrans += translation.z;

	//Rotation
	MQuaternion quaternion(0, 0, 0, 1);
	transformer.getRotation(quaternion);
	dispQuat += "Quaternion rotation: ";
	dispQuat += "X: ";
	dispQuat += quaternion.x;
	dispQuat += "Y: ";
	dispQuat += quaternion.y;
	dispQuat += "Z: ";
	dispQuat += quaternion.z;
	dispQuat += "W: ";
	dispQuat += quaternion.w;

	MGlobal::displayInfo(dispScale);
	MGlobal::displayInfo(dispTrans);
	MGlobal::displayInfo(dispQuat);

	//Fetches names of objects that uses this transformer
	int childCount = transformer.childCount();

	for (int i = 0; i < childCount; ++i)
	{
		if (transformer.child(i).apiType() == MFn::kMesh)
		{
			MFnMesh mesh = transformer.child(i);
			MString meshName = mesh.name();
			MGlobal::displayInfo("Connected Mesh Name: " + meshName);
		}
		else if (transformer.child(i).apiType() == MFn::kCamera)
		{
		}
		else if (transformer.child(i).apiType() == MFn::kLight)
		{
		}
	}

}

void outputMeshData(MFnMesh& mesh)
{
	MString meshName;
	MString displayLenght;
	MString displayCoords;
	
	meshName = mesh.name();
	MGlobal::displayInfo(meshName);

	//Verticis
	MPointArray vts;
	mesh.getPoints(vts);

	displayLenght = "Number of vertisis: ";
	displayLenght += vts.length();
	MGlobal::displayInfo(displayLenght);

	for (int i = 0; i != vts.length(); ++i)
	{
		displayCoords += "X: ";
		displayCoords += vts[i].x;
		displayCoords += "Y: ";
		displayCoords += vts[i].y;
		displayCoords += "Z: ";
		displayCoords += vts[i].z;

	}
	MGlobal::displayInfo(displayCoords);

	//Normals
	MFloatVectorArray nmls;

	mesh.getNormals(nmls);

	displayLenght = "Number of normals: ";
	displayLenght += nmls.length();
	MGlobal::displayInfo(displayCoords);

	displayCoords = "";
	for (int i = 0; i != nmls.length(); ++i)
	{
		displayCoords += "X: ";
		displayCoords += nmls[i].x;
		displayCoords += "Y: ";
		displayCoords += nmls[i].x;
		displayCoords += "Z: ";
		displayCoords += nmls[i].x;

	}
	MGlobal::displayInfo(displayCoords);

	//Texture Coordinates
	bool uv = true;
	MStringArray uvSets;
	mesh.getUVSetNames(uvSets);
	
	if (!uvSets.length() || !mesh.numUVs(uvSets[0]))
	{
		MGlobal::displayInfo("no uvs");
		uv = false;
	}
	else
	{
		displayLenght = ("lenght of UV coordinates");
		displayLenght += uvSets.length();

		displayCoords = "";
		for (int i = 0; i != uvSets.length(); ++i)
		{
			//to see the name
			displayCoords += ("Uv Name: ");
			displayCoords += uvSets[i];

			MFloatArray UCoords;
			MFloatArray VCoords;

			mesh.getUVs(UCoords, VCoords, &uvSets[i]);

			displayCoords += "number of UV: ";
			displayCoords += mesh.numUVs(uvSets[i]);

			for (int j = 0; j != mesh.numUVs(uvSets[i]); ++j)
			{
				int numUVs = mesh.numUVs(uvSets[j]);
				float uC = UCoords[j];
				float vC = VCoords[j];
				displayCoords += UCoords[j];
				displayCoords += " ";
				displayCoords += VCoords[j];
				displayCoords += " ";
			}
		}

		MGlobal::displayInfo(displayCoords);
	}

	//Last function used the bool as return value? Perhaps to not load texture if there was no UV coordinates?
}

void TexturePath(MFnMesh& mesh)
{
	MString texturePath;
	MPlug fullPath = mesh.findPlug("ftn"); //File Texture Name = ftn
	fullPath.getValue(texturePath);

	MGlobal::displayInfo(texturePath);
}

void attributeChangedCallback(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void*clientData)
{
	MStatus res;
	MObject obj(plug.node());

	if (msg & MNodeMessage::AttributeMessage::kAttributeSet)
	{
		if (obj.apiType() == MFn::kMesh)
		{
			MFnMesh mesh(obj);
			MGlobal::displayInfo("Mesh Attribute Changed");
			outputMeshData(mesh);
			TexturePath(mesh);
		}
		else if (obj.apiType() == MFn::kTransform)
		{
			MFnTransform transformer(obj);
			MGlobal::displayInfo("Transfrom Attribute Changed");
			getTransfromData(transformer);
		}

		////Test for simplified shaders
		//if (obj.apiType() == MFn::kBlinn)
		//{
		//	MGlobal::displayInfo("Blinn Material");
		//	MFnBlinnShader ColorValues(obj);
		//	MGlobal::displayInfo(ColorValues.name());
		//}
		
		//plug.node() same as MObject?
		if (plug.node().hasFn(MFn::kBlinn))
		{
			MGlobal::displayInfo("Blinn Material");
			MGlobal::displayInfo(plug.name());
			MFnBlinnShader ColorValues = plug.node();
			MColor color;
			color = ColorValues.color();
			MString displayColor;
			displayColor += color.r;
			displayColor += " ";
			displayColor += color.g;
			displayColor += " ";
			displayColor += color.b;

			MGlobal::displayInfo(displayColor);

		}
		else if (plug.node().hasFn(MFn::kLambert))
		{
			MGlobal::displayInfo("Lambert Material");
			MGlobal::displayInfo(plug.name());
			MFnLambertShader ColorValues = plug.node();
			MColor color;
			color = ColorValues.color();
			MString displayColor;

			displayColor += color.r;
			displayColor += " ";
			displayColor += color.g;
			displayColor += " ";
			displayColor += color.b;

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

	MCallbackId camTranslateId = MUiMessage::add3dViewPostRenderMsgCallback(
		"modelPanel4",
		renderChangeCallback,
		NULL,
		&res
	);


}

EXPORT MStatus uninitializePlugin(MObject obj) 
{
	MFnPlugin plugin(obj);

	MMessage::removeCallbacks(myCallbackArray);
	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}