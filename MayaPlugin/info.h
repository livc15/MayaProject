#pragma once
#include <vector>
#include <DirectXMath.h>

//vertex position, Uv

struct vertex
{
	std::string meshRef;
	float vertPos[3];
};
struct normal
{
	float normalPos[3];
};
struct uv
{
	float uvPos[2];
};

std::vector<std::vector<normal>>	normalList;
std::vector<std::vector<uv>>		uvList;
std::vector<std::vector<vertex>>	vertexList;

struct vertexData
{
	int vertexIndex		= 0;
	float vertexID		= 0;
	float normalID		= 0;
	float uvID			= 0;
};

struct meshData
{
	std::string meshName = "";
	std::vector<vertexData> vertexList;
	
	std::string materialName = "";
};

//material and color
struct materialData
{
	std::string materialName = "";
	float color[3];
	std::string texturePath;
};

struct TransformData
{
	int transformID = 0;
	DirectX::XMMATRIX transformationMatrix;
};

struct CameraData
{
	int cameraID = 0;
	DirectX::XMMATRIX projMatrix;
	DirectX::XMMATRIX viewMatrix;
};