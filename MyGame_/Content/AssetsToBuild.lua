--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/AnimatedShader.shader", arguments = { "fragment" } },
		{ path = "Shaders/Vertex/vertexInputLayout.shader", arguments = { "vertex" } },
	},
	meshes = {
	"Meshes/Mesh1.txt",
	"Meshes/Mesh2.txt",
	"Meshes/Mesh3.txt",
	"Meshes/TestMesh.txt",
	"Meshes/ChairMesh.txt",
	"Meshes/TransparentDonut.txt",
	"Meshes/Plane.txt",
	"Meshes/Arcade.txt",
	},
}
