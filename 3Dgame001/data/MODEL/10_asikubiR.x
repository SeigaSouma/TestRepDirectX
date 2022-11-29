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
 34;
 -0.86370;-4.14011;0.74227;,
 -0.00000;-4.14011;1.04973;,
 -0.00000;-2.93275;1.39845;,
 -0.93592;-2.93275;0.98886;,
 0.86370;-4.14011;0.74227;,
 0.93592;-2.93275;0.98886;,
 1.17116;-4.14011;-0.00000;,
 1.34552;-2.93275;0.00000;,
 0.86370;-4.14011;-0.74227;,
 0.93592;-2.93275;-0.98886;,
 0.00000;-4.14011;-1.04973;,
 0.00000;-2.93275;-1.39845;,
 -0.86370;-4.14011;-0.74227;,
 -0.93592;-2.93275;-0.98886;,
 -1.17116;-4.14011;0.00000;,
 -1.34552;-2.93275;0.00000;,
 -0.00000;-1.59053;1.55678;,
 -0.96572;-1.59053;1.10081;,
 0.96572;-1.59053;1.10081;,
 1.42169;-1.59053;0.00000;,
 0.96572;-1.59053;-1.10081;,
 0.00000;-1.59053;-1.55678;,
 -0.96572;-1.59053;-1.10081;,
 -1.42169;-1.59053;0.00000;,
 0.00000;0.00000;1.83013;,
 -1.00800;0.00000;1.29410;,
 1.00801;0.00000;1.29410;,
 1.54404;0.00000;0.00000;,
 1.00801;0.00000;-1.29410;,
 0.00000;0.00000;-1.83013;,
 -1.00801;0.00000;-1.29410;,
 -1.54404;0.00000;0.00000;,
 0.00000;-4.14011;-0.00000;,
 0.00000;-0.00919;-0.00000;;
 
 40;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 4;3,2,16,17;,
 4;2,5,18,16;,
 4;5,7,19,18;,
 4;7,9,20,19;,
 4;9,11,21,20;,
 4;11,13,22,21;,
 4;13,15,23,22;,
 4;15,3,17,23;,
 4;17,16,24,25;,
 4;16,18,26,24;,
 4;18,19,27,26;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,17,25,31;,
 3;1,0,32;,
 3;4,1,32;,
 3;6,4,32;,
 3;8,6,32;,
 3;10,8,32;,
 3;12,10,32;,
 3;14,12,32;,
 3;0,14,32;,
 3;25,24,33;,
 3;24,26,33;,
 3;26,27,33;,
 3;27,28,33;,
 3;28,29,33;,
 3;29,30,33;,
 3;30,31,33;,
 3;31,25,33;;
 
 MeshMaterialList {
  1;
  40;
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
    "data\\TEXTURE\\murabito_002.png";
   }
  }
 }
 MeshNormals {
  42;
  -0.690521;-0.197455;0.695839;,
  0.000000;-0.248504;0.968631;,
  0.690521;-0.197455;0.695839;,
  0.989733;-0.142932;-0.000000;,
  0.690521;-0.197455;-0.695839;,
  0.000000;-0.248504;-0.968631;,
  -0.690521;-0.197455;-0.695839;,
  -0.989733;-0.142932;0.000000;,
  -0.705224;-0.138466;0.695332;,
  0.000000;-0.178037;0.984024;,
  0.705224;-0.138466;0.695332;,
  0.994996;-0.099916;-0.000000;,
  0.705224;-0.138466;-0.695332;,
  0.000000;-0.178037;-0.984024;,
  -0.705224;-0.138466;-0.695332;,
  -0.994996;-0.099916;0.000000;,
  -0.723037;-0.094793;0.684274;,
  -0.000001;-0.127752;0.991806;,
  0.723037;-0.094795;0.684274;,
  0.997774;-0.066683;-0.000000;,
  0.723038;-0.094795;-0.684274;,
  0.000000;-0.127752;-0.991806;,
  -0.723037;-0.094795;-0.684274;,
  -0.997774;-0.066682;0.000001;,
  -0.728568;-0.109962;0.676090;,
  -0.000001;-0.150955;0.988541;,
  0.728568;-0.109964;0.676089;,
  0.997054;-0.076698;-0.000000;,
  0.728568;-0.109964;-0.676089;,
  0.000000;-0.150956;-0.988540;,
  -0.728568;-0.109964;-0.676089;,
  -0.997054;-0.076697;0.000002;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;-0.000000;,
  0.004311;0.999984;-0.003743;,
  0.000000;0.999987;-0.005021;,
  -0.004311;0.999984;-0.003743;,
  -0.005952;0.999982;0.000000;,
  -0.004311;0.999984;0.003743;,
  0.000000;0.999987;0.005021;,
  0.004311;0.999984;0.003743;,
  0.005952;0.999982;-0.000000;;
  40;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  3;32,32,32;,
  3;32,32,32;,
  3;32,32,32;,
  3;32,32,32;,
  3;32,32,32;,
  3;32,32,32;,
  3;32,32,32;,
  3;32,32,32;,
  3;34,35,33;,
  3;35,36,33;,
  3;36,37,33;,
  3;37,38,33;,
  3;38,39,33;,
  3;39,40,33;,
  3;40,41,33;,
  3;41,34,33;;
 }
 MeshTextureCoords {
  34;
  0.825465;0.544496;,
  0.817925;0.544496;,
  0.817925;0.518544;,
  0.826096;0.518544;,
  0.810386;0.544496;,
  0.809755;0.518544;,
  0.807702;0.544496;,
  0.806179;0.518544;,
  0.810386;0.544496;,
  0.809755;0.518544;,
  0.817925;0.544496;,
  0.817925;0.518544;,
  0.825465;0.544496;,
  0.826096;0.518544;,
  0.828149;0.544496;,
  0.829671;0.518544;,
  0.817925;0.489693;,
  0.826356;0.489693;,
  0.809495;0.489693;,
  0.805514;0.489693;,
  0.809495;0.489693;,
  0.817925;0.489693;,
  0.826356;0.489693;,
  0.830336;0.489693;,
  0.817925;0.455504;,
  0.826725;0.455504;,
  0.809126;0.455504;,
  0.804446;0.455504;,
  0.809126;0.455504;,
  0.817925;0.455504;,
  0.826725;0.455504;,
  0.831404;0.455504;,
  0.817925;0.544496;,
  0.817925;0.455702;;
 }
}
