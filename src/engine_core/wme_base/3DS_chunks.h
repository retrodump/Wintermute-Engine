#ifndef THREEDEE_STUDIO_CHUNKS
#define THREEDEE_STUDIO_CHUNKS


#define	RGB_FLOAT						0x0010
#define	RGB_BYTE						0x0011
#define	RGB_FLOAT_GAMMA					0x0012
#define	RGB_BYTE_GAMMA					0x0013

#define	PERCENT_INT						0x0030
#define	PERCENT_FLOAT					0x0031

#define MAIN3DS 						0x4D4D	// level 1
#define M3DS_VERSION					0x0002
#define MESH_VERSION					0x3D3E

#define EDIT3DS 						0x3D3D	// level 1
#define   NAMED_OBJECT					0x4000	// level 2
#define 	TRIANGLE_MESH				0x4100	// level 3
#define 	  TRIANGLE_VERTEXLIST		0x4110	// level 4
#define 	  TRIANGLE_VERTEXOPTIONS	0x4111	// level 4
#define 	  TRIANGLE_MAPPINGCOORS 	0x4140	// level 4
#define 	  TRIANGLE_MAPPINGSTANDARD	0x4170	// level 4
#define 	  TRIANGLE_FACELIST 		0x4120	// level 4
#define 		TRIANGLE_SMOOTH 		0x4150	// level 5	   
#define 		TRIANGLE_MATERIAL		0x4130	// level 5 
#define 	  TRI_LOCAL 				0x4160	// level 4
#define 	  TRI_VISIBLE				0x4165	// level 4
#define 	CHUNK_CAMERA				0x4700	// level 3

#define		CHUNK_LIGHT					0x4600
#define			LIGHT_SPOTLIGHT			0x4610
#define			LIGHT_IS_OFF			0x4620

#define 	INT_PERCENTAGE				0x0030 
#define 	MASTER_SCALE				0x0100 

#define   EDIT_MATERIAL 				0xAFFF	// level 2
#define 	MAT_NAME01					0xA000	// level 3


#define 	  TEXTURE_MAP				0xA200	// level 4?
#define 	  TEXTURE_MAP2				0xA33A
#define 	  OPACITY_MAP				0xA210
#define 	  BUMP_MAP					0xA230
#define 	  SHININESS_MAP				0xA33C
#define 	  SPECULAR_MAP				0xA204
#define 	  SELF_ILUM_MAP				0xA33D
#define 	  REFLECTION_MAP			0xA220
#define 	  MASK_TEXTURE_MAP			0xA33E
#define 	  MASK_TEXTURE_MAP2			0xA340
#define 	  MASK_OPACITY_MAP			0xA342
#define 	  MASK_BUMP_MAP				0xA344
#define 	  MASK_SHININESS_MAP		0xA346
#define 	  MASK_SPECULAR_MAP			0xA348
#define 	  MASK_SELF_ILUM_MAP		0xA34A
#define 	  MASK_REFLECTION_MAP		0xA34C

#define 		MAPPING_NAME			0xA300	// level 5?

#define			MAPPING_U_SCALE			0xA356
#define			MAPPING_V_SCALE			0xA354

#define			MAPPING_U_OFFSET		0xA358
#define			MAPPING_V_OFFSET		0xA35A

#define			MAPPING_ROTATION		0xA35C
 
#define 		MATERIAL_AMBIENT		0xA010
#define 		MATERIAL_DIFFUSE		0xA020
#define 		MATERIAL_SPECULAR		0xA030
#define 		MATERIAL_SHININESS		0xA040
#define 		MATERIAL_SHIN_STRENGTH	0xA041

#define 		MAPPING_PARAMETERS		0xA351
#define 		BLUR_PERCENTAGE 		0xA353

#define 		TRANS_PERCENT			0xA050
#define 		TRANS_FALLOFF_PERCENT	0xA052
#define 		REFLECTION_BLUR_PER 	0xA053
#define 		RENDER_TYPE 			0xA100

#define 		SELF_ILLUM				0xA084
#define 		WIRE_THICKNESS			0xA087
#define 		IN_TRANC				0xA08A
#define 		SOFTEN					0xA08C
	 
#define KEYFRAMER						0xB000	// level 1
#define	  KEYFRAMER_AMBIENT_INFO		0xB001
#define   KEYFRAMER_MESH_INFO			0xB002
#define   KEYFRAMER_CAMERA_INFO			0xB003
#define   KEYFRAMER_CAMERA_TARGET_INFO	0xB004
#define   KEYFRAMER_OMNI_INFO			0xB005
#define   KEYFRAMER_SPOT_TARGET_INFO	0xB006
#define   KEYFRAMER_SPOT_INFO			0xB007

#define   KEYFRAME_START_AND_END		0xB008
#define   KEYFRAME_HEADER				0xB00A

#define 	POS_TRACK_TAG				0xB020
#define 	ROT_TRACK_TAG				0xB021 
#define 	SCL_TRACK_TAG				0xB022
#define 	FOV_TRACK_TAG				0xB023
#define 	ROLL_TRACK_TAG				0xB024
#define 	COLOR_TRACK_TAG				0xB025
#define 	MORPH_TRACK_TAG				0xB026
#define 	HOTSPOT_TRACK_TAG			0xB027
#define 	FALLOFF_TRACK_TAG			0xB028
#define 	HIDE_TRACK_TAG				0xB029

#define 	PIVOT			0xB013 

#define 	NODE_HDR		0xB010
#define 	NODE_ID 		0xB030
#define 	KFCURTIME		0xB009

#endif // THREEDEE_STUDIO_CHUNKS
