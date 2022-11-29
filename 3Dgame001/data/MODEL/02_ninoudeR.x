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
 48;
 12.24400;4.50362;3.82150;,
 5.79444;2.90852;2.26763;,
 4.91197;1.88573;3.31703;,
 12.07428;1.36146;4.97462;,
 4.77533;-0.64406;2.26763;,
 11.90457;-1.78071;3.82150;,
 4.71873;-1.69192;0.00000;,
 12.02909;-3.69049;-0.00000;,
 12.02909;-3.69049;-0.00000;,
 4.71873;-1.69192;0.00000;,
 4.77533;-0.64406;-2.26763;,
 11.90457;-1.78071;-3.82150;,
 4.91197;1.88573;-3.31703;,
 12.07428;1.36146;-4.97462;,
 5.79444;2.90852;-2.26763;,
 12.24400;4.50362;-3.82150;,
 5.85103;3.95638;0.00000;,
 12.57348;6.38889;0.00000;,
 12.57348;6.38889;0.00000;,
 5.85103;3.95638;0.00000;,
 12.07428;1.36146;4.97462;,
 12.07428;1.36146;0.00000;,
 12.24400;4.50362;3.82150;,
 11.90457;-1.78071;3.82150;,
 12.02909;-3.69049;-0.00000;,
 11.90457;-1.78071;-3.82150;,
 12.07428;1.36146;-4.97462;,
 12.24400;4.50362;-3.82150;,
 12.57348;6.38889;0.00000;,
 -1.29698;1.89225;2.26763;,
 -0.60424;-0.46807;0.00000;,
 -0.02087;-0.11967;3.31703;,
 0.82711;-2.39461;2.26764;,
 1.95557;-2.52450;0.00000;,
 0.82711;-2.39461;-2.26764;,
 -0.02087;-0.11967;-3.31703;,
 -1.29697;1.89224;-2.26763;,
 -1.01146;3.22092;0.00000;,
 -1.29698;1.89225;2.26763;,
 -0.02087;-0.11967;3.31703;,
 0.82711;-2.39461;2.26764;,
 1.95557;-2.52450;0.00000;,
 1.95557;-2.52450;0.00000;,
 0.82711;-2.39461;-2.26764;,
 -0.02087;-0.11967;-3.31703;,
 -1.29697;1.89224;-2.26763;,
 -1.01146;3.22092;0.00000;,
 -1.01146;3.22092;0.00000;;
 
 32;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;8,9,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;18,19,1,0;,
 3;20,21,22;,
 3;23,21,20;,
 3;24,21,23;,
 3;25,21,24;,
 3;26,21,25;,
 3;27,21,26;,
 3;28,21,27;,
 3;22,21,28;,
 3;29,30,31;,
 3;31,30,32;,
 3;32,30,33;,
 3;33,30,34;,
 3;34,30,35;,
 3;35,30,36;,
 3;36,30,37;,
 3;37,30,29;,
 4;1,38,39,2;,
 4;2,39,40,4;,
 4;4,40,41,6;,
 4;9,42,43,10;,
 4;10,43,44,12;,
 4;12,44,45,14;,
 4;14,45,46,16;,
 4;19,47,38,1;;
 
 MeshMaterialList {
  1;
  32;
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
  43;
  -0.266361;0.740113;0.617483;,
  -0.252485;0.126638;0.959278;,
  -0.257919;-0.652858;0.712218;,
  -0.260611;-0.965444;0.000000;,
  -0.257919;-0.652858;-0.712218;,
  -0.252485;0.126638;-0.959278;,
  -0.266361;0.740113;-0.617483;,
  -0.338029;0.941136;0.000000;,
  -0.196450;0.749086;0.632675;,
  -0.122292;0.124892;0.984605;,
  0.002341;-0.694720;0.719277;,
  0.053193;-0.998584;-0.000000;,
  0.002341;-0.694720;-0.719277;,
  -0.122292;0.124892;-0.984605;,
  -0.196450;0.749086;-0.632675;,
  -0.233964;0.972245;-0.000000;,
  0.998545;-0.053933;-0.000000;,
  -0.957574;-0.288095;0.007341;,
  -0.877902;-0.435030;0.200091;,
  -0.887698;-0.460427;-0.000000;,
  -0.784734;-0.619076;-0.030625;,
  -0.626287;-0.779592;0.000000;,
  -0.784734;-0.619076;0.030625;,
  -0.877903;-0.435029;-0.200091;,
  -0.957574;-0.288094;-0.007343;,
  -0.993962;-0.109721;-0.000002;,
  0.998545;-0.053934;-0.000000;,
  0.996908;-0.076371;0.018520;,
  0.999334;-0.031448;0.018522;,
  0.999960;-0.008945;-0.000000;,
  0.999334;-0.031448;-0.018522;,
  0.996908;-0.076371;-0.018520;,
  0.995106;-0.098809;-0.000000;,
  -0.125986;0.754144;0.644511;,
  -0.134786;0.583149;0.801105;,
  0.158932;-0.374476;0.913514;,
  0.266865;-0.687729;0.675138;,
  0.349210;-0.937044;-0.000000;,
  0.266865;-0.687729;-0.675138;,
  0.158932;-0.374476;-0.913514;,
  -0.134787;0.583150;-0.801105;,
  -0.125987;0.754144;-0.644511;,
  -0.123855;0.992300;-0.000001;;
  32;
  4;0,8,9,1;,
  4;1,9,10,2;,
  4;2,10,11,3;,
  4;3,11,12,4;,
  4;4,12,13,5;,
  4;5,13,14,6;,
  4;6,14,15,7;,
  4;7,15,8,0;,
  3;26,16,27;,
  3;28,16,26;,
  3;29,16,28;,
  3;30,16,29;,
  3;26,16,30;,
  3;31,16,26;,
  3;32,16,31;,
  3;27,16,32;,
  3;17,19,18;,
  3;18,19,20;,
  3;20,19,21;,
  3;21,19,22;,
  3;22,19,23;,
  3;23,19,24;,
  3;24,19,25;,
  3;25,19,17;,
  4;8,33,34,9;,
  4;9,35,36,10;,
  4;10,36,37,11;,
  4;11,37,38,12;,
  4;12,38,39,13;,
  4;13,40,41,14;,
  4;14,41,42,15;,
  4;15,42,33,8;;
 }
 MeshTextureCoords {
  48;
  0.665889;0.898224;,
  0.709649;0.862210;,
  0.719185;0.865802;,
  0.680715;0.930104;,
  0.731121;0.891470;,
  0.695541;0.961985;,
  0.736065;0.902101;,
  0.703256;0.983229;,
  0.702956;0.825920;,
  0.735880;0.744509;,
  0.730919;0.733840;,
  0.695215;0.804602;,
  0.718941;0.708083;,
  0.680337;0.772610;,
  0.709372;0.704479;,
  0.665460;0.740618;,
  0.704410;0.693810;,
  0.655232;0.723297;,
  0.655698;0.880963;,
  0.704705;0.851578;,
  0.813874;0.846984;,
  0.813874;0.827609;,
  0.828144;0.841073;,
  0.799603;0.841073;,
  0.793692;0.826802;,
  0.799603;0.812531;,
  0.813874;0.806620;,
  0.828144;0.812531;,
  0.834056;0.826802;,
  0.828144;0.765010;,
  0.813874;0.780088;,
  0.813874;0.759099;,
  0.799603;0.765010;,
  0.793692;0.779281;,
  0.799603;0.793551;,
  0.813874;0.799462;,
  0.828144;0.793551;,
  0.834056;0.779281;,
  0.754510;0.814802;,
  0.756118;0.846574;,
  0.761324;0.877620;,
  0.755471;0.888252;,
  0.755354;0.730611;,
  0.761227;0.719943;,
  0.756003;0.688789;,
  0.754389;0.656906;,
  0.746877;0.645137;,
  0.747024;0.803075;;
 }
}