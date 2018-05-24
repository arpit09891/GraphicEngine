--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	meshes =
	{
		"Meshes/Floor.details",
		"Meshes/Roof.details",
		"Meshes/Cube.details",
		"Meshes/Enemy.details",
		"Meshes/Player.details",
		"Meshes/Bullet.details",
		"Meshes/Disc.details"
	},
	shaders =
	{
		{ path = "Shaders/Vertex/vertexInputLayout_Mesh.hlsl", arguments = { "vertex" } },
		{ path = "Shaders/Vertex/vertexInputLayout_Sprite.hlsl", arguments = { "vertex" } },
		{ path = "Shaders/Vertex/MeshVertex.shader", arguments = { "vertex" } },
		{ path = "Shaders/Vertex/SpriteVertex.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/MeshFragment.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/MeshFragmentTranslucent.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/SpriteFragment.shader", arguments = { "fragment" } }
	},
	textures =
	{		
		"Textures/eae.png",
		"Textures/lara.png",
		"Textures/wood.jpg",
		"Textures/hull.jpg",
		"Textures/Red.png",
		"Textures/Yellow.png",
		"Textures/Green.png",
		"Textures/Blue.png",
		"Textures/Space.gif",
		"Textures/Pink.png",
		"Textures/YouWin.png",
		"Textures/Silver.png"
	},
}
