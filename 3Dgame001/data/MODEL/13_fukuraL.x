xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 26;
 -2.31230;-11.15400;-3.44075;,
 -3.20176;-3.71817;-4.20837;,
 -0.32528;-3.71817;-6.01259;,
 -0.32528;-11.15400;-4.96600;,
 3.49556;-3.71817;-4.20837;,
 2.52719;-11.15400;-3.44075;,
 -3.20176;-3.71817;4.20837;,
 -2.31230;-11.15400;3.44075;,
 -0.32528;-11.15400;4.96600;,
 -0.32528;-3.71817;6.01259;,
 2.52719;-11.15400;3.44075;,
 3.49556;-3.71817;4.20837;,
 -4.09351;-11.15400;0.00000;,
 -0.32528;-11.15400;0.00000;,
 4.53005;-11.15400;-0.00000;,
 5.49842;-3.71817;0.00000;,
 -4.98296;-3.71817;0.00000;,
 -3.68216;-0.00026;-4.58801;,
 0.00000;-0.00026;-6.39223;,
 -5.46336;-0.00026;0.00000;,
 -3.68217;-0.00026;4.58801;,
 -0.00000;-0.00026;6.39223;,
 3.96081;-0.00026;4.58801;,
 5.96367;-0.00026;0.00000;,
 3.96081;-0.00026;-4.58801;,
 0.00000;-0.00026;-0.00000;;
 
 24;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;6,7,8,9;,
 4;9,8,10,11;,
 4;7,12,13,8;,
 4;8,13,14,10;,
 4;12,0,3,13;,
 4;13,3,5,14;,
 4;14,15,11,10;,
 4;5,4,15,14;,
 4;7,6,16,12;,
 4;12,16,1,0;,
 4;1,17,18,2;,
 4;16,19,17,1;,
 4;6,20,19,16;,
 4;21,20,6,9;,
 4;22,21,9,11;,
 4;15,23,22,11;,
 4;4,24,23,15;,
 4;2,18,24,4;,
 4;25,19,20,21;,
 4;23,25,21,22;,
 4;18,25,23,24;,
 4;17,19,25,18;;
 
 MeshMaterialList {
  1;
  24;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\���[�V�����r���[�A\\Ver.1.1\\data\\TEXTURE\\murabito_002.png";
   }
  }
 }
 MeshNormals {
  26;
  -0.637262;-0.551803;-0.537969;,
  -0.042301;-0.800075;-0.598407;,
  0.580115;-0.559892;-0.591598;,
  -0.749728;-0.133080;-0.648226;,
  -0.053207;-0.145509;-0.987925;,
  0.697398;-0.144122;-0.702043;,
  -0.749729;-0.133081;0.648226;,
  -0.053207;-0.145509;0.987925;,
  0.697398;-0.144122;0.702043;,
  -0.637262;-0.551803;0.537969;,
  -0.042301;-0.800075;0.598407;,
  0.580115;-0.559892;0.591598;,
  -0.621635;-0.783307;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.609298;-0.792941;-0.000000;,
  0.988307;-0.152475;-0.000000;,
  -0.989191;-0.146631;-0.000000;,
  -0.037317;-0.134128;-0.990261;,
  -0.742342;-0.128067;-0.657669;,
  -0.988844;-0.148953;-0.000000;,
  -0.742342;-0.128068;0.657668;,
  -0.037317;-0.134128;0.990261;,
  0.702014;-0.144013;0.697450;,
  0.989054;-0.147554;-0.000000;,
  0.702014;-0.144013;-0.697450;,
  0.000000;1.000000;0.000000;;
  24;
  4;0,3,4,1;,
  4;1,4,5,2;,
  4;6,9,10,7;,
  4;7,10,11,8;,
  4;9,12,13,10;,
  4;10,13,14,11;,
  4;12,0,1,13;,
  4;13,1,2,14;,
  4;14,15,8,11;,
  4;2,5,15,14;,
  4;9,6,16,12;,
  4;12,16,3,0;,
  4;3,18,17,4;,
  4;16,19,18,3;,
  4;6,20,19,16;,
  4;21,20,6,7;,
  4;22,21,7,8;,
  4;15,23,22,8;,
  4;5,24,23,15;,
  4;4,17,24,5;,
  4;25,25,25,25;,
  4;25,25,25,25;,
  4;25,25,25,25;,
  4;25,25,25,25;;
 }
 MeshTextureCoords {
  26;
  0.670715;0.623070;,
  0.663707;0.548624;,
  0.686371;0.548624;,
  0.686371;0.623070;,
  0.716476;0.548624;,
  0.708846;0.623070;,
  0.663707;0.548624;,
  0.670715;0.623070;,
  0.686371;0.623070;,
  0.686371;0.548624;,
  0.708846;0.623070;,
  0.716476;0.548624;,
  0.656681;0.623070;,
  0.686371;0.623070;,
  0.724627;0.623070;,
  0.732256;0.548624;,
  0.649673;0.548624;,
  0.659922;0.511402;,
  0.688934;0.511402;,
  0.645888;0.511402;,
  0.659922;0.511402;,
  0.688934;0.511402;,
  0.720142;0.511402;,
  0.735922;0.511402;,
  0.720142;0.511402;,
  0.688934;0.511402;;
 }
}