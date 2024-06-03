// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: modelClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: MAIN PURPOSE:
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#include "platform.h"

#include "womadriverclass.h"
#include "modelClass.h"
#include "DXmodelClass.h"
#include "GLmodelClass.h"
#include "DXshaderClass.h"
#include "DX11Class.h"
#include "fileLoader.h"
#include "OSmain_dir.h"
#include "dxWinSystemClass.h"
#include "mem_leak.h"

#ifdef NOTES
GEOMETRY
--------
#		- comment
mtllib	- material library filename

v		- vert position		(30) COLOR shader
vt		- vert tex coords	(31) TEXTURE shader
vn		- vert normal		(32) LIGHT shader

f		- defines the faces	(poligons) 1 poligon of 3...6 vert OR 1 triangle 3 vert
g		- defines a group	(Subset)
usemtl	- which material to use

MATERIAL
--------
#		- comment
Ka		- Ambient Color			(32) LIGHT shader
Kd		- diffuse color			(32) LIGHT shader
Ke		- emissive color factors (self-ilumination)

*Ni		- optical_density	
		ADVANCED TRANSPARENCY: 
		The values can range from 0.001 to 10. 
		A value of "1.0" means that light does not bend as it passes through an object. (neutral)
		The optical density of the surface: This is also known as "index of refraction". Glass is about 1.5 (default)

Tr		- Check for transparency (alpha value)	(33) ALFA MAP and ALFA COLOR
d		- for transparency (1 - alpha value)	(33) ALFA MAP and ALFA COLOR

map_Kd	- Diffuse map	(31)
map_d	- Alpha map     (33)

illum	- Illumination					(34 SPECULAR + SHININESS) CH51 if ( illumination == 2 ) hasSpecular = true
Ks		- Specular Color 				(34 SPECULAR + SHININESS) CH51
Ns		- Shininess (Specular Power)	(34 SPECULAR + SHININESS) CH51

map_bump - Bump map						(35) The BUMP MAP

newmtl	- Declare new material

// ----------------------------
// NOT INCLUDED FOR NOW:
// ----------------------------
illum	x

*0. Color on and Ambient off
*1. Color on and Ambient on
2. Highlight on (hasSpecular)
*3. Reflection on and Ray trace on
*4. Transparency: Glass on, Reflection: Ray trace on
*5. Reflection: Fresnel on and Ray trace on
*6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
*7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
*8. Reflection on and Ray trace off
*9. Transparency: Glass on, Reflection: Ray trace off
*10. Casts shadows onto invisible surfaces

Ia			ambient light
I			light intensity
It			intensity from transmitted direction
Tf r g b	transmission filter:

map_Ks lenna.tga           # specular color texture map
map_Ns lenna_spec.tga      # specular highlight component

#endif
const bool isRHCoordSys = true;	// SET default

ModelClass::ModelClass()
{
}

ModelClass::~ModelClass()
{
}

// --------------------------------------------------------------------------------------------
bool ModelClass::LoadOBJ(/*DXmodelClass*/ void* dxmodelClass, SHADER_TYPE shader_type, void* g_driver, STRING filename, bool castShadow, bool renderShadow, UINT instanceCount)
// --------------------------------------------------------------------------------------------
{	HRESULT hr = 0;

	//---------------------------------------------------------------------
	WOMA_LOGManager_DebugMSG( TEXT("OBJ Loading: %s with shader: [%d]\n"), (TCHAR*)(filename+TEXT(" ")).c_str(), shader_type);
	obj3d.fileNameOnly=filename;

	// Add full path:
	STRING newfilename = WOMA::LoadFile((TCHAR*)filename.c_str());

	// Get path to Mat. Lib.
	STRING MathLibPath = newfilename;

	//Open file: filename
	// ---------------------
	IFSTREAM fileIn ((TCHAR*)newfilename.c_str());
	if (!fileIn) 
		{ WOMA::WomaMessageBox((TCHAR*)newfilename.c_str(), TEXT("Error, Could not load: ")); return FALSE; }

	obj3d.m_vertexCount = 0;		//totalVerts
	TCHAR lastToken = 0;

	//////////////////////
	// Geometry
	//////////////////////

	//Check to see if the file was opened
	if (fileIn)
	{
		while( fileIn && (WOMA::game_state < GAME_STOP) )
		{			
			checkChar = fileIn.get();	//Get next char

			switch (checkChar)
			{		
			case '#':	// Comment
				checkChar = fileIn.get();
				while(checkChar != '\n')
					checkChar = fileIn.get();
				break;

			case 'v':	//Get Vertex Descriptions
				checkChar = fileIn.get();
				if(checkChar == ' ')			//v - vert position
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;	//Store the next three types

					if(isRHCoordSys)			//If model is from an RH Coord System
						obj3d.vertPos.push_back(XMFLOAT3( vx, vy, vz * -1.0f));	//Invert the Z axis
					else
						obj3d.vertPos.push_back(XMFLOAT3( vx, vy, vz));
				}
				//41
				if(checkChar == 't')			//vt - vert tex coords
				{			
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;		//Store next two types

					if(isRHCoordSys)			//If model is from an RH Coord System
						obj3d.vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f-vtcv));	//Reverse the "v" axis
					else
						obj3d.vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));

					obj3d.hasTexCoord = true;	//We know the model uses texture coords
				}
				//99
				//Since we compute the normals later, we don't need to check for normals
				//In the file, but i'll do it here anyway
				if(checkChar == 'n')				//vn - vert normal
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;	//Store next three types

					if(isRHCoordSys)	//If model is from an RH Coord System
						obj3d.vertNorm.push_back(XMFLOAT3( vnx, vny, vnz * -1.0f ));	//Invert the Z axis
					else
						obj3d.vertNorm.push_back(XMFLOAT3( vnx, vny, vnz ));

					obj3d.hasNorm = true;	//We know the model defines normals
				}
				break;

				//New group (Subset)
			case 'g':	//g - defines a group
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					obj3d.meshSubsetIndexStart.push_back(vIndex);		//Start index for this subset
					obj3d.meshSubsets++;
					lastToken = L'g';
				}
				break;

				//Get Face Index
			case 'f':	//f - defines the faces
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					face = TEXT("");
					STRING/*std::wstring*/ VertDef;	//Holds one vertex definition at a time
					obj3d.triangleCount = 0;

					checkChar = fileIn.get();
					while(checkChar != '\n')
					{
						face += checkChar;			//Add the char to our face string
						checkChar = fileIn.get();	//Get the next Character
						if(checkChar == ' ')		//If its a space...
							obj3d.triangleCount++;		//Increase our triangle count
					}

					//Check for space at the end of our face string
					if(face[face.length()-1] == ' ')
						obj3d.triangleCount--;	//Each space adds to our triangle count

					obj3d.triangleCount -= 1;		//Ever vertex in the face AFTER the first two are new faces

					STRINGSTREAM ss(face);

					if(face.length() > 0)
					{
						int firstVIndex = 0, lastVIndex = 0;	//Holds the first and last vertice's index

						for(int i = 0; i < 3; ++i)		//First three vertices (first triangle)
						{
							ss >> VertDef;	//Get vertex definition (vPos/vTexCoord/vNorm)

							STRING vertPart;
							int whichPart = 0;		//#(vPos, vTexCoord, or vNorm)

							//Parse this string
							for(UINT j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')	//If there is no divider "/", add a char to our vertPart
									vertPart += VertDef[j];

								//If the current char is a divider "/", or its the last character in the string
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									STRINGSTREAM wstringToInt(vertPart);	//Used to convert wstring to int
									// -----------------------------------------------
									if(whichPart == 0)	//#If vPos
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}
									// -----------------------------------------------
									else if(whichPart == 1)	//#If vTexCoord
									{
										if(vertPart != TEXT(""))	//Check to see if there even is a tex coord
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;	//subtract one since c++ arrays start with 0, and obj start with 1
										}
										else	//If there is no tex coord, make a default
											vertTCIndexTemp = 0;

										//If the cur. char is the second to last in the string, then
										//there must be no normal, so set a default normal
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;
									}								
									// -----------------------------------------------
									else if(whichPart == 2)	//#If vNorm
									{
										STRINGSTREAM /*std::wstringstream*/ wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1
									}

									vertPart = TEXT("");	//Get ready for next vertex part
									whichPart++;			//Move on to next vertex part					
								}
							}

							//Check to make sure there is at least one subset
							if(obj3d.meshSubsets == 0)
							{
								obj3d.meshSubsetIndexStart.push_back(vIndex);		//Start index for this subset
								obj3d.meshSubsets++;
							}

							//Avoid duplicate vertices
							bool vertAlreadyExists = false;
							if(obj3d.m_vertexCount >= 3)	//Make sure we at least have one triangle to check
							{
								//Loop through all the vertices
								for(UINT iCheck = 0; iCheck < obj3d.m_vertexCount; ++iCheck)
								{
									//If the vertex position and texture coordinate in memory are the same
									//As the vertex position and texture coordinate we just now got out
									//of the obj file, we will set this faces vertex index to the vertex's
									//index value in memory. This makes sure we don't create duplicate vertices
									if(vertPosIndexTemp == obj3d.vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == obj3d.vertTCIndex[iCheck])
										{
										#if DX9sdk
                                        if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
                                            indices16.push_back((WORD)iCheck);		//Set index for this vertex
                                        else
										#endif
											obj3d.indices32.push_back(iCheck);		//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
										}
										obj3d.m_indexCount++;
									}
								}
							}

							//If this vertex is not already in our vertex arrays, put it there
							if(!vertAlreadyExists)
							{
								obj3d.vertPosIndex.push_back(vertPosIndexTemp);
								obj3d.vertTCIndex.push_back(vertTCIndexTemp);
								obj3d.vertNormIndex.push_back(vertNormIndexTemp);
								obj3d.m_vertexCount++;	//We created a new vertex
								#if DX9sdk
								if ( (g_driver) && (g_driver->ShaderVersionH < 4.0f) )
									obj3d.indices16.push_back((WORD) /*totalVerts*/m_vertexCount - 1);	//Set index for this vertex
								else
								#endif
									obj3d.indices32.push_back(obj3d.m_vertexCount-1);	//Set index for this vertex
								obj3d.m_indexCount++;
							}							

							//If this is the very first vertex in the face, we need to
							//make sure the rest of the triangles use this vertex
							if(i == 0)
							{
								#if DX9sdk
                                if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
                                    firstVIndex = obj3d.indices16[vIndex];	//The first vertex index of this FACE
                                else
								#endif
								    firstVIndex = obj3d.indices32[vIndex];	//The first vertex index of this FACE

							}

							//If this was the last vertex in the first triangle, we will make sure
							//the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
							if(i == 2)
							{				
								#if DX9sdk
                                if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
                                    lastVIndex = obj3d.indices16[vIndex];	//The last vertex index of this TRIANGLE
                                else
								#endif
								    lastVIndex = obj3d.indices32[vIndex];	//The last vertex index of this TRIANGLE
							}
							vIndex++;	//Increment index count
						}

						obj3d.meshTriangles++;	//One triangle down

						//If there are more than three vertices in the face definition, we need to make sure
						//we convert the face to triangles. We created our first triangle above, now we will
						//create a new triangle for every new vertex in the face, using the very first vertex
						//of the face, and the last vertex from the triangle before the current triangle
						for(UINT l = 0; l < obj3d.triangleCount-1; ++l)	//Loop through the next vertices to create new triangles
						{
							//First vertex of this triangle (the very first vertex of the face too)
							#if DX9sdk
                            if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
                                indices16.push_back((WORD) firstVIndex);			//Set index for this vertex
                            else
							#endif
								obj3d.indices32.push_back(firstVIndex);			//Set index for this vertex
							vIndex++;

							//Second Vertex of this triangle (the last vertex used in the tri before this one)
							#if DX9sdk
                            if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
								obj3d.indices16.push_back((WORD) lastVIndex);			//Set index for this vertex
                            else
							#endif
								obj3d.indices32.push_back(lastVIndex);			//Set index for this vertex
							vIndex++;

							//Get the third vertex for this triangle
							ss >> VertDef;

							STRING vertPart;
							int whichPart = 0;

							//Parse this string (same as above)
							for(UINT j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')
									vertPart += VertDef[j];
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									STRINGSTREAM /*std::wistringstream*/ wstringToInt(vertPart);

									if(whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if(whichPart == 1)
									{
										if(vertPart != TEXT(""))
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;

									}								
									else if(whichPart == 2)
									{
										STRINGSTREAM /*std::wistringstream*/ wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = TEXT("");
									whichPart++;							
								}
							}					

							//Check for duplicate vertices
							bool vertAlreadyExists = false;
							if(obj3d.m_vertexCount >= 3)	//Make sure we at least have one triangle to check
							{
								for(UINT iCheck = 0; iCheck < obj3d.m_vertexCount; ++iCheck)
								{
									if(vertPosIndexTemp == obj3d.vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == obj3d.vertTCIndex[iCheck])
										{
											#if DX9sdk
                                            if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
												obj3d.indices16.push_back((WORD) iCheck);			//Set index for this vertex
                                            else
											#endif
												obj3d.indices32.push_back(iCheck);			//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
											obj3d.m_indexCount++;
										}
									}
								}
							}

							if(!vertAlreadyExists)
							{
								obj3d.vertPosIndex.push_back(vertPosIndexTemp);
								obj3d.vertTCIndex.push_back(vertTCIndexTemp);
								obj3d.vertNormIndex.push_back(vertNormIndexTemp);
								obj3d.m_vertexCount++;					//New vertex created, add to total verts
								#if DX9sdk
                                if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
									obj3d.indices16.push_back((WORD) obj3d.m_vertexCount - 1);		//Set index for this vertex
                                else
								#endif
									obj3d.indices32.push_back(obj3d.m_vertexCount-1);		//Set index for this vertex
								obj3d.m_indexCount++;
							}

							//Set the second vertex for the next triangle to the last vertex we got		
							#if DX9sdk
                            if ((g_driver) && (g_driver->ShaderVersionH < 4.0f))
                                lastVIndex = indices16[vIndex];	//The last vertex index of this TRIANGLE
                            else
							#endif
							    lastVIndex = obj3d.indices32[vIndex];	//The last vertex index of this TRIANGLE

							obj3d.meshTriangles++;	//New triangle defined
							vIndex++;		
						}
					}
				}
				break;

			case 'm':	//mtllib - material library filename
				checkChar = fileIn.get();
				if(checkChar == 't')
				{
					checkChar = fileIn.get();
					if(checkChar == 'l')
					{
						checkChar = fileIn.get();
						if(checkChar == 'l')
						{
							checkChar = fileIn.get();
							if(checkChar == 'i')
							{
								checkChar = fileIn.get();
								if(checkChar == 'b')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										//Store the material libraries file name
										fileIn >> meshMatLib;
									}
								}
							}
						}
					}
				}

				break;

			case 'u':	//usemtl - which material to use
				checkChar = fileIn.get();
				if(checkChar == 's')
				{
					checkChar = fileIn.get();
					if(checkChar == 'e')
					{
						checkChar = fileIn.get();
						if(checkChar == 'm')
						{
							checkChar = fileIn.get();
							if(checkChar == 't')
							{
								checkChar = fileIn.get();
								if(checkChar == 'l')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										meshMaterialsTemp = TEXT("");	//Make sure this is cleared
										fileIn >> meshMaterialsTemp; //Get next type (string)

										if (lastToken == 'u') 
										{	// We have a "usemtl" but we dont have a 'g' for it, so "force" it:
											obj3d.meshSubsetIndexStart.push_back(vIndex);		//Start index for this subset
											obj3d.meshSubsets++;
										}

										obj3d.meshMaterials.push_back(meshMaterialsTemp);
										lastToken = 'u';
									}
								}
							}
						}
					}
				}
				break;

			default:				
				break;
			}
		}
	}
	else	//If we could not open the file
	{
		//create message
		STRING message = TEXT("Could not open: ");
		message += filename;

		return false;
	}

	////////////////////////////////////////////////////////////////////
	// VERTEX LOAD DONE!
	////////////////////////////////////////////////////////////////////

	if (WOMA::game_state == GAME_STOP)
		return true; // Stop the loading immediately

	obj3d.meshSubsetIndexStart.push_back(vIndex); //There won't be another index start after our last subset, so set it here

	ASSERT (obj3d.meshSubsetIndexStart.size() >= 2); // If fail... wrong OBJ format!

	//sometimes "g" is defined at the very top of the file, then again before the first group of faces.
	//This makes sure the first subset does not conatain "0" indices.
	if(obj3d.meshSubsetIndexStart[1] == 0)
	{
		obj3d.meshSubsetIndexStart.erase(obj3d.meshSubsetIndexStart.begin()+1);
		obj3d.meshSubsets--;
	}

	//Make sure we have a default for the tex coord and normal
	//if one or both are not specified
	if(!obj3d.hasNorm)
		obj3d.vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	if(!obj3d.hasTexCoord)
		obj3d.vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));

	//Close the obj file, and open the mtl file
	fileIn.close();

	// Get Path to Mat. Lib.
	// ---------------------
	{
	STRING temp = MathLibPath;
	int indexCh = (int)temp.find_last_of('\\');
	if (indexCh == -1)
		indexCh = (int)temp.find_last_of('/');
	indexCh++;
	STRING path = MathLibPath;
	path = path.substr(0, indexCh);
	//_tcscpy_s((TCHAR*)&MathLibPath[0], sizeof(MathLibPath), (TCHAR*)path.c_str());
	MathLibPath = path;

	meshMatLib = MathLibPath + meshMatLib;
	}

	IFSTREAM fileMtl((TCHAR*)meshMatLib.c_str());

	//std::wstring lastStringRead;
	//bool SKIP_MATERIALS = false;
#if !defined(STANDALONE)
	if (meshMatLib.find(TEXT("none")) == 0) //Special Case for Billboards: dont have a fixed texture.
	{
		SurfaceMaterial tempMat;
		obj3d.material.push_back(tempMat);

		strcpy_s(obj3d.material[0].matName, "none");

		obj3d.material[0].hasTexture = false;
		obj3d.material[0].texArrayIndex = 0;

		//ALFA-MAP:
			obj3d.material[0].alfaMap11 = NULL;
		obj3d.material[0].transparent = false;

		// SPECULAR + SHININESS
		#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
			obj3d.material[0].specularColor = XMFLOAT3(0.0f, 0.0f, 0.0f);
			obj3d.material[0].bSpecular = false;
			obj3d.material[0].nShininess = 0;
        #endif

		//BUMP
			obj3d.material[0].hasNormMap = false;
			obj3d.material[0].normMapTexArrayIndex = 0;

		obj3d.textureNameArray.push_back(TEXT("none"));
		obj3d.material[0].texArrayIndex = 0;
		obj3d.material[0].hasTexture = true;

		//SKIP_MATERIALS = true;
		goto SKIP;
	}


#else
	// On Standalone Converter: mount the fullpath name plus mtl filename.
	const TCHAR* fname = filename.c_str();
	PathRemoveFileSpec((TCHAR*) fname);
	wstring fullname = fname;
	fullname += TEXT("\\");
	fullname += meshMatLib;

	IFSTREAM fileMtl((TCHAR*)fullname.c_str());
	//fileMtl.open(fullname.c_str());
#endif

	int matCount = (int)obj3d.material.size();	//total materials

	//kdset - If our diffuse color was not set, we can use the ambient color (which is usually the same)
	//If the diffuse color WAS set, then we don't need to set our diffuse color to ambient
	bool kdset = false;

	if (fileMtl) //if ((fileMtl) && (!SKIP_MATERIALS))
	{
		while( fileMtl && (WOMA::game_state < GAME_STOP) )
		{
			checkChar = fileMtl.get();	//Get next char

			switch (checkChar)
			{
            	//Check for comment
			case '#':
				checkChar = fileMtl.get();
				while(checkChar != '\n')
					checkChar = fileMtl.get();
				break;

			case 'K':	//Set k - diffuse color
				checkChar = fileMtl.get();

				//Kd
				if(checkChar == 'd')	//Diffuse Color
				{
					checkChar = fileMtl.get();	//remove space

					fileMtl >> obj3d.material[matCount-1].diffuseColor.x;
					fileMtl >> obj3d.material[matCount-1].diffuseColor.y;
					fileMtl >> obj3d.material[matCount-1].diffuseColor.z;

					kdset = true;
					break;
				}

				//Ka - Ambient Color (We'll store it in diffuse if there isn't a diffuse already)
				if(checkChar == 'a')	
				{					
					checkChar = fileMtl.get();	//remove space
					if(!kdset)
					{
						fileMtl >> obj3d.material[matCount-1].ambientColor.x;
						fileMtl >> obj3d.material[matCount-1].ambientColor.y;
						fileMtl >> obj3d.material[matCount-1].ambientColor.z;
					}
					break;
				}

				//Ke - emissive color factors (self-ilumination)
				if(checkChar == 'e')	
				{					
					checkChar = fileMtl.get();	//remove space
					if(!kdset)
					{
						fileMtl >> obj3d.material[matCount-1].emissiveColor.x;
						fileMtl >> obj3d.material[matCount-1].emissiveColor.y;
						fileMtl >> obj3d.material[matCount-1].emissiveColor.z;
					}
					break;
				}

				// Ks - Specular Color
			#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
				if(checkChar == 's')	{
					checkChar = fileMtl.get();	//remove space
					fileMtl >> obj3d.material[matCount-1].specularColor.x;//r
					fileMtl >> obj3d.material[matCount-1].specularColor.y;//g
					fileMtl >> obj3d.material[matCount-1].specularColor.z;//b

					if (obj3d.material[matCount-1].specularColor.x + obj3d.material[matCount-1].specularColor.y + obj3d.material[matCount-1].specularColor.z == 0)
						obj3d.material[matCount-1].bSpecular = false;
					break;
				}

			case 'N':	//Ns - Shininess
				checkChar = fileMtl.get();
				if(checkChar == 's')	{
					fileMtl >> obj3d.material[matCount-1].nShininess;
				}
				break;

			case 'i':
				checkChar = fileMtl.get();
				if(checkChar == 'l')
				{
					checkChar = fileMtl.get();
					if(checkChar == 'l')
					{
						checkChar = fileMtl.get();
						if(checkChar == 'u')
						{
							checkChar = fileMtl.get();
							if(checkChar == 'm')
							{
								checkChar = fileMtl.get();
								int illumination;
								fileMtl >> illumination;
								obj3d.material[matCount-1].bSpecular = ( illumination == 2 );
							}
						}
					}
				}
			#endif
			
			//ALFA MAP/COLOR:
			case 'T': // Tr: Check for transparency
				checkChar = fileMtl.get();
				if(checkChar == 'r')
				{
					checkChar = fileMtl.get();	//remove space
					float Transparency;
					fileMtl >> Transparency;

					obj3d.material[matCount-1].diffuseColor.w = Transparency;

					if(Transparency > 0.0f)
						obj3d.material[matCount-1].transparent = true;
				}
				break;

			case 'd': // d: Some obj files specify d for transparency
				checkChar = fileMtl.get();
				if(checkChar == ' ')
				{
					float Transparency;
					fileMtl >> Transparency;

					//'d' - 0 being most transparent, and 1 being opaque, opposite of Tr
					Transparency = 1.0f - Transparency;

					obj3d.material[matCount-1].diffuseColor.w = Transparency;

					if(Transparency > 0.0f)
						obj3d.material[matCount-1].transparent = true;
				}
				break;

				//Get the diffuse map (texture)
			case 'm':
				checkChar = fileMtl.get();
				if(checkChar == 'a')
				{
					checkChar = fileMtl.get();
					if(checkChar == 'p')
					{
						checkChar = fileMtl.get();
						if(checkChar == '_')
						{
							//map_Kd - Diffuse map
							checkChar = fileMtl.get();
							if(checkChar == 'K')
							{
								checkChar = fileMtl.get();
								if(checkChar == 'd')
								{
									STRING fileNamePath;

									fileMtl.get();	//Remove whitespace between map_Kd and file

									//Get the file path - We read the pathname char by char since
									//pathnames can sometimes contain spaces, so we will read until
									//we find the file extension
									bool texFilePathEnd = false;
									while(!texFilePathEnd)
									{
										checkChar = fileMtl.get();

										fileNamePath += checkChar;

										if(checkChar == '.')
										{
											for(int i = 0; i < 3; ++i)
												fileNamePath += fileMtl.get();

											texFilePathEnd = true;
										}							
									}

									//check if this texture has already been loaded
									bool alreadyLoaded = false;
									for(UINT i = 0; i < obj3d.textureNameArray.size(); ++i)
									{
										if(fileNamePath == obj3d.textureNameArray[i])
										{
											alreadyLoaded = true;
											obj3d.material[matCount-1].texArrayIndex = i;
											obj3d.material[matCount-1].hasTexture = true;
										}
									}

									//if the texture is not already loaded, load it now
									if(!alreadyLoaded)
									{
										ID3D11ShaderResourceView* tempMeshSRV = NULL;
										fileNamePath = MathLibPath + fileNamePath;	// TO Support TEMP:
										fileNamePath = WOMA::LoadFile ((TCHAR*)fileNamePath.c_str());
										
										hr = S_OK;
										#if !defined(STANDALONE)
											//[TEMMPLATE] LOAD TEXTURE DX11:
											#define m_driver11 ((DirectX::DX11Class*)SystemHandle->driverList[SystemHandle->AppSettings->DRIVER])
											LOADTEXTURE(fileNamePath.c_str(), tempMeshSRV);
										#endif

										if(SUCCEEDED(hr))
										{
											obj3d.material[matCount-1].texArrayIndex = (int)((DXmodelClass*)dxmodelClass)->meshSRV11.size();
											obj3d.material[matCount-1].hasTexture = true;

											obj3d.textureNameArray.push_back(fileNamePath.c_str());
											((DXmodelClass*)dxmodelClass)->meshSRV11.push_back(tempMeshSRV);
										} else {
											return false;
										}
										
									}	
								}
							}

							//ALFA MAP/COLOR:
							//map_d - alpha map
							else if(checkChar == 'd')
							{
								//Alpha maps are usually the same as the diffuse map
								//So we will assume that for now by only enabling
								//transparency for this material, as we will already
								//be using the alpha channel in the diffuse map
								obj3d.material[matCount-1].transparent = true;

                                STRING fileNamePath;

                                fileMtl.get();	//Remove whitespace between map_Kd and file

                                //Get the file path - We read the pathname char by char since
                                //pathnames can sometimes contain spaces, so we will read until
                                //we find the file extension
                                bool texFilePathEnd = false;
                                while (!texFilePathEnd)
                                {
                                    checkChar = fileMtl.get();

                                    fileNamePath += checkChar;

                                    if (checkChar == '.')
                                    {
                                        for (int i = 0; i < 3; ++i)
                                            fileNamePath += fileMtl.get();

                                        texFilePathEnd = true;
                                    }
                                }

                                fileNamePath = MathLibPath + fileNamePath;	// TO Support TEMP:
                                fileNamePath = WOMA::LoadFile((TCHAR*) fileNamePath.c_str());
                                hr = S_OK;

                                ID3D11ShaderResourceView* tempMeshSRV=NULL;

                            #if !defined(STANDALONE)
							  if (SystemHandle->AppSettings->DRIVER == DRIVER_DX11 || SystemHandle->AppSettings->DRIVER == DRIVER_DX9) 
							  {
								//[TEMMPLATE] LOAD TEXTURE DX11:
								#define m_driver11 ((DirectX::DX11Class*)SystemHandle->driverList[SystemHandle->AppSettings->DRIVER])
								LOADTEXTURE((TCHAR*)fileNamePath.c_str(), tempMeshSRV);
								obj3d.material[matCount - 1].alfaMap11 = tempMeshSRV;
							  }

                            #endif
							}

                            ///BUMP:
                            //map_bump - bump map (we're usinga normal map though)
                            else if (checkChar == 'b')
                            {
                                checkChar = fileMtl.get();
                                if (checkChar == 'u')
                                {
                                    checkChar = fileMtl.get();
                                    if (checkChar == 'm')
                                    {
                                        checkChar = fileMtl.get();
                                        if (checkChar == 'p')
                                        {
                                            STRING fileNamePath;

                                            fileMtl.get();	//Remove whitespace between map_bump and file

                                            //Get the file path - We read the pathname char by char since
                                            //pathnames can sometimes contain spaces, so we will read until
                                            //we find the file extension
                                            bool texFilePathEnd = false;
                                            while (!texFilePathEnd)
                                            {
                                                checkChar = fileMtl.get();

                                                fileNamePath += checkChar;

                                                if (checkChar == '.')
                                                {
                                                    for (int i = 0; i < 3; ++i)
                                                        fileNamePath += fileMtl.get();

                                                    texFilePathEnd = true;
                                                }
                                            }

                                            //check if this texture has already been loaded
                                            bool alreadyLoaded = false;
                                            for (UINT i = 0; i < obj3d.textureNameArray.size(); ++i)
                                            {
                                                if (fileNamePath == obj3d.textureNameArray[i])
                                                {
                                                    alreadyLoaded = true;
													obj3d.material[matCount - 1].normMapTexArrayIndex = i;
													obj3d.material[matCount - 1].hasNormMap = true;
													obj3d.ModelHASNormals = true;
                                                }
                                            }

                                            //if the texture is not already loaded, load it now
                                            if (!alreadyLoaded)
                                            {
                                                ID3D11ShaderResourceView* tempMeshSRV=NULL;
                                                fileNamePath = MathLibPath + fileNamePath;	// TO Support TEMP:
                                                fileNamePath = WOMA::LoadFile((TCHAR*) fileNamePath.c_str());

												//[TEMMPLATE] LOAD TEXTURE DX11:
												#define m_driver11 ((DirectX::DX11Class*)SystemHandle->driverList[SystemHandle->AppSettings->DRIVER])
												LOADTEXTURE(fileNamePath.c_str(), tempMeshSRV);
												obj3d.textureNameArray.push_back(fileNamePath.c_str());
												obj3d.material[matCount - 1].normMapTexArrayIndex = (int)((DXmodelClass*)dxmodelClass)->meshSRV11.size();
												((DXmodelClass*)dxmodelClass)->meshSRV11.push_back(tempMeshSRV);
												obj3d.material[matCount - 1].hasNormMap = true;
												obj3d.ModelHASNormals = true;
                                            }
                                        }
                                    }
                                }
                            }
                            ///NEW
						}
					}
				}
				break;

			case 'n':	//newmtl - Declare new material
				checkChar = fileMtl.get();
				if(checkChar == 'e')
				{
					checkChar = fileMtl.get();
					if(checkChar == 'w')
					{
						checkChar = fileMtl.get();
						if(checkChar == 'm')
						{
							checkChar = fileMtl.get();
							if(checkChar == 't')
							{
								checkChar = fileMtl.get();
								if(checkChar == 'l')
								{
									checkChar = fileMtl.get();
									if(checkChar == ' ')
									{
										//New material, set its defaults
										SurfaceMaterial tempMat = { 0 };
										obj3d.material.push_back(tempMat);

										#ifdef UNICODE
										TCHAR matName[50];
										fileMtl >> matName;
										wtoa(material[matCount].matName, matName, MAX_STR_LEN);
										#else
										fileMtl >> obj3d.material[matCount].matName;
										#endif

										obj3d.material[matCount].diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
										obj3d.material[matCount].ambientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
										obj3d.material[matCount].emissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

										obj3d.material[matCount].texArrayIndex = 0;
										obj3d.material[matCount].hasTexture = false;

										//ALFA MAP/COLOR:
										obj3d.material[matCount].alfaMap11 = NULL;
										obj3d.material[matCount].transparent = false;

										// SPECULAR + SHININESS
									#if defined RENDER_OBJ_WITH_SPECULAR_SHININESS
										obj3d.material[matCount].bSpecular = false;
										obj3d.material[matCount].specularColor = XMFLOAT3(0.0f, 0.0f, 0.0f);
										obj3d.material[matCount].nShininess = 0;
                                    #endif

										//BUMP
										obj3d.material[matCount].hasNormMap = false;
										obj3d.material[matCount].normMapTexArrayIndex = 0;

										matCount++;
										kdset = false;
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}	
	else
	{
		STRING message = TEXT("Could not open: ");
		message += meshMatLib;

		return false;
	}

SKIP:
	fileMtl.close();
	if (WOMA::game_state == GAME_STOP)
		return true; // Stop the loading immediately

	return true;
}



bool ModelClass::CreateObject(/*DXmodelClass*/ void* XmodelClass, TCHAR* objectName, void* g_driver, SHADER_TYPE shader_type, STRING filename, bool castShadow, bool renderShadow)
{
	// Make Sure that: "meshSubsets" size are equal to "meshMaterials" size
	if (obj3d.meshSubsets != obj3d.meshMaterials.size())
	{
		WOMA::WomaMessageBox((TCHAR*)filename.c_str(), TEXT("LoadOBJ: Warning meshSubsets != meshMaterials.size"));
	}

	//Set the subsets material to the index value of the its material in our material array
	for (UINT i = 0; i < obj3d.meshSubsets; ++i)
	{
		//bool hasMat = false;
		for (UINT j = 0; j < obj3d.material.size(); ++j)
		{
#ifdef UNICODE
			CHAR buffer[MAX_STR_LEN] = { 0 }; wtoa(buffer, meshMaterials[i].c_str(), MAX_STR_LEN);
			if (strcmp(buffer, material[j].matName) == 0)
#else
			if (obj3d.meshMaterials[i] == obj3d.material[j].matName)
#endif
				obj3d.subsetMaterialArray.push_back(j);
		}
	}
	////////////////////////////////////////////////////////////////////
	//MATERIALS DONE!
	////////////////////////////////////////////////////////////////////
	//Move data to advanced model types:
	if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
		((DXmodelClass*)XmodelClass)->obj3d = obj3d;
	else
		((GLmodelClass*)XmodelClass)->obj3d = obj3d;

// --------------------------------------------------------------------------------------------
// Post Read Actions:
// --------------------------------------------------------------------------------------------

	///////////////////////// COMPUTE NORMALS //////////////////////////
	// If computeNormals was set to true then we will create our own
	// normals, if it was set to false we will use the obj files normals
	////////////////////////////////////////////////////////////////////
	//SHADER_NORMAL_BUMP?
	if (obj3d.ModelHASNormals) // Forced to be TRUE on CH51!
	{
		std::vector<ModelNormalBumpVertexType> vertices;
		ModelNormalBumpVertexType tempVert;

		//Create our vertices using the information we got 
		//from the file and store them in a vector
		XMVECTOR min, max;

		min = XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, 0.0f);
		max = XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f);

		for (UINT j = 0; j < obj3d.m_vertexCount/*totalVerts*/; ++j)
		{
			tempVert.x = obj3d.vertPos[obj3d.vertPosIndex[j]].x;
			tempVert.y = obj3d.vertPos[obj3d.vertPosIndex[j]].y;
			tempVert.z = obj3d.vertPos[obj3d.vertPosIndex[j]].z;

			if (tempVert.x > max.m128_f32[0] /*max.x*/) max.m128_f32[0] /*max.x*/ = tempVert.x;
			if (tempVert.y > max.m128_f32[1] /*max.y*/) max.m128_f32[1] /*max.y*/ = tempVert.y;
			if (tempVert.z > max.m128_f32[2] /*max.z*/) max.m128_f32[2] /*max.z*/ = tempVert.z;

			if (tempVert.x < min.m128_f32[0] /*min.x*/) min.m128_f32[0] /*min.x*/ = tempVert.x;
			if (tempVert.y < min.m128_f32[1] /*min.y*/) min.m128_f32[1] /*min.x*/ = tempVert.y;
			if (tempVert.z < min.m128_f32[2] /*min.z*/) min.m128_f32[2] /*min.x*/ = tempVert.z;

			tempVert.tu = obj3d.vertTexCoord[obj3d.vertTCIndex[j]].x;
			tempVert.tv = obj3d.vertTexCoord[obj3d.vertTCIndex[j]].y;

			if (obj3d.vertNormIndex[j] < (int)obj3d.vertNorm.size())				//Make it more rubust
			{
				tempVert.nx = obj3d.vertNorm[obj3d.vertNormIndex[j]].x;
				tempVert.ny = obj3d.vertNorm[obj3d.vertNormIndex[j]].y;
				tempVert.nz = obj3d.vertNorm[obj3d.vertNormIndex[j]].z;
			}

			vertices.push_back(tempVert);
		}

		std::vector<XMFLOAT3> tempNormal;

		//normalized and unnormalized normals
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//tangent stuff
		std::vector<XMFLOAT3> tempTangent;
		XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float tcU1, tcV1, tcU2, tcV2;

		//Used to get vectors (sides) from the position of the verts
		float vecX, vecY, vecZ;

		//Two edges of our triangle
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Compute face normals
		for (UINT i = 0; i < obj3d.meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,2)
			vecX = vertices[obj3d.indices32[(i * 3)]].x - vertices[obj3d.indices32[(i * 3) + 2]].x;
			vecY = vertices[obj3d.indices32[(i * 3)]].y - vertices[obj3d.indices32[(i * 3) + 2]].y;
			vecZ = vertices[obj3d.indices32[(i * 3)]].z - vertices[obj3d.indices32[(i * 3) + 2]].z;
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

			//Get the vector describing another edge of our triangle (edge 2,1)
			vecX = vertices[obj3d.indices32[(i * 3) + 2]].x - vertices[obj3d.indices32[(i * 3) + 1]].x;
			vecY = vertices[obj3d.indices32[(i * 3) + 2]].y - vertices[obj3d.indices32[(i * 3) + 1]].y;
			vecZ = vertices[obj3d.indices32[(i * 3) + 2]].z - vertices[obj3d.indices32[(i * 3) + 1]].z;
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

			//Cross multiply the two edge vectors to get the un-normalized face normal
			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

			tempNormal.push_back(unnormalized);			//Save unormalized normal (for normal averaging)

			////////////////////////////
			//Find first texture coordinate edge 2d vector
			tcU1 = vertices[obj3d.indices32[(i * 3)]].tu - vertices[obj3d.indices32[(i * 3) + 2]].tu;
			tcV1 = vertices[obj3d.indices32[(i * 3)]].tv - vertices[obj3d.indices32[(i * 3) + 2]].tv;

			//Find second texture coordinate edge 2d vector
			tcU2 = vertices[obj3d.indices32[(i * 3) + 2]].tu - vertices[obj3d.indices32[(i * 3) + 1]].tu;
			tcV2 = vertices[obj3d.indices32[(i * 3) + 2]].tv - vertices[obj3d.indices32[(i * 3) + 1]].tv;

			//Find tangent using both tex coord edges and position edges
			tangent.x = (tcV1 * XMVectorGetX(edge1) - tcV2 * XMVectorGetX(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
			tangent.y = (tcV1 * XMVectorGetY(edge1) - tcV2 * XMVectorGetY(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
			tangent.z = (tcV1 * XMVectorGetZ(edge1) - tcV2 * XMVectorGetZ(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));

			tempTangent.push_back(tangent);
		}

		//Compute vertex normals (normal Averaging)
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		int facesUsing = 0;
		float tX, tY, tZ;	//temp axis variables

		//Go through each vertex
		for (UINT i = 0; i < /*totalVerts*/obj3d.m_vertexCount; ++i)
		{
			//Check which triangles use this vertex
			for (UINT j = 0; j < obj3d.meshTriangles; ++j)
			{
				if (obj3d.indices32[j * 3] == i ||
					obj3d.indices32[(j * 3) + 1] == i ||
					obj3d.indices32[(j * 3) + 2] == i)
				{
					tX = XMVectorGetX(normalSum) + tempNormal[j].x;
					tY = XMVectorGetY(normalSum) + tempNormal[j].y;
					tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

					normalSum = XMVectorSet(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum

					//We can reuse tX, tY, tZ to sum up tangents
					tX = XMVectorGetX(tangentSum) + tempTangent[j].x;
					tY = XMVectorGetY(tangentSum) + tempTangent[j].y;
					tZ = XMVectorGetZ(tangentSum) + tempTangent[j].z;

					tangentSum = XMVectorSet(tX, tY, tZ, 0.0f); //sum up face tangents using this vertex

					facesUsing++;
				}
			}

			normalSum = normalSum / (float)facesUsing;	//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
			normalSum = XMVector3Normalize(normalSum);	//Normalize the normalSum vector

			tangentSum = tangentSum / (float)facesUsing;		//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
			tangentSum = XMVector3Normalize(tangentSum);//Normalize the normalSum vector and tangent

			//Store the normal in our current vertex
			vertices[i].nx = XMVectorGetX(normalSum);
			vertices[i].ny = XMVectorGetY(normalSum);
			vertices[i].nz = XMVectorGetZ(normalSum);

			////////////////////////////
			vertices[i].tx = XMVectorGetX(tangentSum);
			vertices[i].ty = XMVectorGetY(tangentSum);
			vertices[i].tz = XMVectorGetZ(tangentSum);

			//Clear normalSum, tangentSum and facesUsing for next vertex
			normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			facesUsing = 0;
		}

		if (shader_type == 0)
			shader_type = SHADER_NORMAL_BUMP;

		if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
			((DXmodelClass*)XmodelClass)->LoadBump((TCHAR*)filename.c_str(), g_driver, shader_type, &obj3d.textureNameArray, &vertices, &obj3d.indices32);
		else
			((GLmodelClass*)XmodelClass)->LoadBump((TCHAR*)filename.c_str(), g_driver, /*shader_type*/SHADER_NORMAL_BUMP, &obj3d.textureNameArray, &vertices, &obj3d.indices32);

	}
	else
		//SHADER_TEXTURE_LIGHT
		//SHADER_TEXTURE_LIGHT_RENDERSHADOW
		if (obj3d.hasNorm || shader_type == SHADER_TEXTURE_LIGHT)
		{
			std::vector<ModelTextureLightVertexType> modelTextureLightVertex;
			ModelTextureLightVertexType tempVert;
			for (UINT j = 0; j < obj3d.m_vertexCount; ++j)
			{
				tempVert.x = obj3d.vertPos[obj3d.vertPosIndex[j]].x;
				tempVert.y = obj3d.vertPos[obj3d.vertPosIndex[j]].y;
				tempVert.z = obj3d.vertPos[obj3d.vertPosIndex[j]].z;

				tempVert.tu = obj3d.vertTexCoord[obj3d.vertTCIndex[j]].x;
				tempVert.tv = obj3d.vertTexCoord[obj3d.vertTCIndex[j]].y;

				if (obj3d.vertNormIndex[j] < (int)obj3d.vertNorm.size()) //Make it more rubust
				{
					tempVert.nx = obj3d.vertNorm[obj3d.vertNormIndex[j]].x;
					tempVert.ny = obj3d.vertNorm[obj3d.vertNormIndex[j]].y;
					tempVert.nz = obj3d.vertNorm[obj3d.vertNormIndex[j]].z;
				}

				modelTextureLightVertex.push_back(tempVert);
			}

			if (shader_type == 0)
				shader_type = (renderShadow) ? SHADER_TEXTURE_LIGHT_RENDERSHADOW : SHADER_TEXTURE_LIGHT;

			if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
				((DXmodelClass*)XmodelClass)->LoadLight((TCHAR*)filename.c_str(), g_driver, shader_type, &obj3d.textureNameArray, &modelTextureLightVertex, &obj3d.indices32);
			else
				((GLmodelClass*)XmodelClass)->LoadLight((TCHAR*)filename.c_str(), g_driver, /*shader_type*/(renderShadow) ? SHADER_TEXTURE_LIGHT_RENDERSHADOW : SHADER_TEXTURE_LIGHT, & obj3d.textureNameArray, & modelTextureLightVertex, & obj3d.indices32);
		}
		else
			// SHADER_TEXTURE - 31
			if (obj3d.hasTexCoord)
			{
				std::vector<ModelTextureVertexType> modelTextureVertex;
				ModelTextureVertexType tempVert;
				for (UINT j = 0; j < obj3d.m_vertexCount; ++j)
				{
					tempVert.x = obj3d.vertPos[obj3d.vertPosIndex[j]].x;
					tempVert.y = obj3d.vertPos[obj3d.vertPosIndex[j]].y;
					tempVert.z = obj3d.vertPos[obj3d.vertPosIndex[j]].z;

					tempVert.tu = obj3d.vertTexCoord[obj3d.vertTCIndex[j]].x;
					tempVert.tv = obj3d.vertTexCoord[obj3d.vertTCIndex[j]].y;

					modelTextureVertex.push_back(tempVert);
				}

				if (shader_type == 0)
					shader_type = SHADER_TEXTURE;

				if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
					((DXmodelClass*)XmodelClass)->LoadTexture((TCHAR*)filename.c_str(), g_driver, shader_type, &obj3d.textureNameArray, &modelTextureVertex, &obj3d.indices32);
				else
					((GLmodelClass*)XmodelClass)->LoadTexture((TCHAR*)filename.c_str(), g_driver, /*shader_type*/ SHADER_TEXTURE, &obj3d.textureNameArray, &modelTextureVertex, &obj3d.indices32);
			}
			else
			// SHADER_COLOR - 30
			{
				std::vector<ModelColorVertexType> modelColorVertex(obj3d.vertPos.size());
				ModelColorVertexType tempVert;

				for (UINT i = 0; i < obj3d.meshSubsets; ++i) // Parts of Object (sub meshes)
				{
					int indexStart = obj3d.meshSubsetIndexStart[i];
					int indexDrawAmount = obj3d.meshSubsetIndexStart[i + 1] - obj3d.meshSubsetIndexStart[i];

					for (int j = 0; j < indexDrawAmount; ++j)
					{
						tempVert.x = obj3d.vertPos[obj3d.vertPosIndex[obj3d.indices32[indexStart + j]]].x;
						tempVert.y = obj3d.vertPos[obj3d.vertPosIndex[obj3d.indices32[indexStart + j]]].y;
						tempVert.z = obj3d.vertPos[obj3d.vertPosIndex[obj3d.indices32[indexStart + j]]].z;

						//Color:
						tempVert.r = obj3d.material[obj3d.subsetMaterialArray[i]].diffuseColor.x;
						tempVert.g = obj3d.material[obj3d.subsetMaterialArray[i]].diffuseColor.y;
						tempVert.b = obj3d.material[obj3d.subsetMaterialArray[i]].diffuseColor.z;
						tempVert.a = 1;

						modelColorVertex[obj3d.vertPosIndex[obj3d.indices32[indexStart + j]]] = tempVert;
					}
				}

				if (shader_type == 0)
					shader_type = SHADER_COLOR;

				if (DXsystemHandle->AppSettings->DRIVER != DRIVER_GL3)
					((DXmodelClass*)XmodelClass)->LoadColor((TCHAR*)filename.c_str(), g_driver, shader_type, &modelColorVertex, &obj3d.indices32);
				else
					((GLmodelClass*)XmodelClass)->LoadColor((TCHAR*)filename.c_str(), g_driver, /*shader_type*/ SHADER_COLOR, &modelColorVertex, &obj3d.indices32);
			}
	// VERTICES/INDEXES DONE

	return true;
}

