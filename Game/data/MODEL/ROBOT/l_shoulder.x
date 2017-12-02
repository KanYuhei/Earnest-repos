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
 251;
 -2.74772;1.98984;-0.83676;,
 -2.74772;1.98880;0.74788;,
 -1.35508;1.98880;0.74788;,
 -1.35508;1.98984;-0.83676;,
 -2.74772;0.57620;-0.85316;,
 -2.74772;0.56836;0.73156;,
 -1.35508;0.59200;0.73124;,
 -1.35508;0.60080;-0.85348;,
 -2.91896;-0.97336;-0.85576;,
 -2.91896;-0.98120;0.72896;,
 -2.91896;-4.64288;-0.87852;,
 -2.91896;-4.64392;0.70608;,
 0.99832;0.59200;0.73124;,
 0.99832;0.60080;-0.85348;,
 -2.91896;-5.46476;1.54356;,
 -2.91896;-5.45420;-1.70572;,
 -2.91896;-6.80012;-1.71008;,
 -2.91896;-6.81068;1.53920;,
 -2.91896;-7.75488;-0.76148;,
 -2.91896;-7.75928;0.58440;,
 -1.75040;-5.46476;1.54356;,
 -1.35508;-5.63028;1.14772;,
 -1.35508;-4.80896;0.30972;,
 -1.75040;-4.64392;0.70608;,
 -1.35508;-5.63028;1.14772;,
 -1.75040;-5.46476;1.54356;,
 -1.75040;-6.81068;1.53920;,
 -1.35508;-6.64564;1.14444;,
 -1.35508;-6.64564;1.14444;,
 -1.75040;-7.75928;0.58440;,
 -1.35508;-7.36344;0.42196;,
 -1.35508;-7.36344;0.42196;,
 -1.75040;-7.75488;-0.76148;,
 -1.35508;-7.36016;-0.59648;,
 -1.35508;-7.36016;-0.59648;,
 -1.75040;-6.80012;-1.71008;,
 -1.35508;-6.63768;-1.31428;,
 -1.35508;-6.63768;-1.31428;,
 -1.75040;-5.45420;-1.70572;,
 -1.35508;-5.62072;-1.31096;,
 -1.35508;-5.62072;-1.31096;,
 -1.75040;-4.64288;-0.87852;,
 -1.35508;-4.80988;-0.48432;,
 -1.35508;-4.80988;-0.48432;,
 -1.74948;-0.60260;-0.85504;,
 -1.35508;-0.99916;-0.46104;,
 -1.35508;-1.00356;0.33312;,
 -1.75128;-0.60960;0.72968;,
 0.60216;-1.00356;0.33316;,
 0.99832;-0.61004;0.72980;,
 0.99832;-0.60212;-0.85496;,
 0.60300;-0.99916;-0.46016;,
 -2.91896;-5.46476;1.54356;,
 -1.35508;-5.62072;-1.31096;,
 -1.35508;-5.63028;1.14772;,
 -1.35508;-6.64564;1.14444;,
 -1.35508;-6.63768;-1.31428;,
 -1.35508;-4.80896;0.30972;,
 -1.35508;-4.80896;0.30972;,
 -1.35508;-4.80988;-0.48432;,
 -1.35508;-7.36344;0.42196;,
 -1.35508;-7.36016;-0.59648;,
 -1.23060;3.23180;-2.67932;,
 -1.22952;0.82296;-2.67932;,
 -2.23900;0.82296;-2.67932;,
 -2.24364;3.23180;-2.67932;,
 -1.23116;4.43500;1.57656;,
 -1.23116;4.43500;0.00000;,
 -2.24596;4.43500;0.00000;,
 -2.24596;4.43500;1.58468;,
 -1.22952;0.82296;2.67932;,
 -1.23060;3.23180;2.67932;,
 -2.24364;3.23180;2.67932;,
 -2.23900;0.82296;2.67932;,
 -3.60332;4.51352;0.00156;,
 -4.97956;1.40632;-0.00300;,
 -4.79140;1.40360;1.00324;,
 -3.40628;4.52972;1.00136;,
 -1.22896;-0.45704;-2.67932;,
 -2.23652;-0.45704;-2.67932;,
 -5.14124;0.82296;0.00024;,
 -5.14124;-0.45704;0.00024;,
 -4.86532;-0.45704;1.59248;,
 -4.85728;0.82296;1.59232;,
 -1.22896;-0.45704;2.67932;,
 -2.23652;-0.45704;2.67932;,
 -1.23116;4.43500;-1.57412;,
 -2.24596;4.43500;-1.57412;,
 -3.46236;4.52616;-0.97936;,
 -4.83324;1.40288;-0.98024;,
 -4.92264;0.82296;-1.57412;,
 -4.93220;-0.45704;-1.57412;,
 -1.23116;4.43500;-1.57412;,
 -2.24596;4.43500;-1.57412;,
 -3.99300;0.82296;-2.67932;,
 -4.92264;0.82296;-1.57412;,
 -3.36888;4.43500;-1.57412;,
 -2.99836;3.23180;-2.67932;,
 -3.99300;-0.45704;-2.67932;,
 -1.22720;-4.30868;-2.67932;,
 -1.21388;-4.30868;-1.57412;,
 -3.26664;-4.30868;-1.57412;,
 -2.22908;-4.30868;-2.67932;,
 -1.21388;-4.30868;1.57556;,
 -1.22720;-4.30868;2.67932;,
 -2.22908;-4.30868;2.67932;,
 -3.26664;-4.30868;1.58036;,
 -3.99300;-0.45704;2.67932;,
 -3.99300;0.82296;2.67932;,
 -3.29744;4.43500;1.59308;,
 -4.85728;0.82296;1.59232;,
 -2.99836;3.23180;2.67932;,
 -1.23116;4.43500;1.57656;,
 -2.24596;4.43500;1.58468;,
 -2.84904;4.51500;1.01100;,
 -2.84824;4.51500;-0.00224;,
 -3.26664;-4.30868;0.00000;,
 -4.88948;-4.30868;1.59308;,
 -5.14124;-4.30868;0.00000;,
 -2.84824;4.51500;-0.99048;,
 -4.96092;-4.30868;-1.57412;,
 -3.36888;4.43500;-1.57412;,
 -3.99300;-4.30868;-2.67932;,
 -3.99300;-4.30868;2.67932;,
 -3.29744;4.43500;1.59308;,
 -1.22772;-3.19848;-2.67932;,
 -2.23124;-3.19848;-2.67932;,
 -3.99300;-3.19848;-2.67932;,
 -4.95264;-3.19848;-1.57412;,
 -5.14124;-3.19848;0.00008;,
 -4.88252;-3.19848;1.59292;,
 -3.99300;-3.19848;2.67932;,
 -2.23124;-3.19848;2.67932;,
 -1.22772;-3.19848;2.67932;,
 -0.92336;-3.47968;1.48104;,
 -0.92336;-3.47968;-1.48160;,
 -0.92336;-1.43260;-1.25320;,
 -0.92336;-1.43260;1.25128;,
 -1.22720;-4.30868;-2.67932;,
 -2.22908;-4.30868;-2.67932;,
 -2.22908;-4.30868;2.67932;,
 -1.22720;-4.30868;2.67932;,
 -5.37400;0.92444;0.00512;,
 -5.45304;1.12072;0.00668;,
 -5.24620;1.11860;-1.47108;,
 -5.15044;0.92216;-1.60296;,
 -5.08392;0.92184;1.63044;,
 -5.18464;1.11860;1.50440;,
 -5.18464;1.11860;1.50440;,
 -5.08392;0.92184;1.63044;,
 -3.45884;4.68484;1.62032;,
 -3.57092;4.83932;1.48524;,
 -2.24596;4.68564;1.58468;,
 -2.39532;4.83500;1.44112;,
 -3.52944;4.68236;-1.59452;,
 -3.63968;4.83824;-1.45576;,
 -2.39532;4.83500;-1.42828;,
 -2.24596;4.68564;-1.57412;,
 -5.15044;0.92216;-1.60296;,
 -5.24620;1.11860;-1.47108;,
 -2.24596;4.68564;0.00000;,
 -2.39532;4.83500;0.00000;,
 -2.39532;4.83500;1.44112;,
 -2.24596;4.68564;1.58468;,
 -2.24596;4.68564;-1.57412;,
 -2.39532;4.83500;-1.42828;,
 -5.33488;1.39796;0.00452;,
 -5.13576;1.47540;0.00084;,
 -4.98904;1.47160;-1.00276;,
 -5.16392;1.39508;-1.17012;,
 -5.11432;1.39520;1.20508;,
 -4.94596;1.47208;1.03248;,
 -4.94596;1.47208;1.03248;,
 -5.11432;1.39520;1.20508;,
 -3.59840;4.84792;1.18404;,
 -3.51740;4.70300;1.02136;,
 -2.69752;4.83500;1.15068;,
 -2.84740;4.68644;1.00940;,
 -3.65952;4.84472;-1.15420;,
 -3.57512;4.70112;-0.99492;,
 -2.84704;4.68616;-0.98928;,
 -2.69712;4.83500;-1.13356;,
 -5.16392;1.39508;-1.17012;,
 -4.98904;1.47160;-1.00276;,
 -2.69712;4.83500;0.00000;,
 -2.84704;4.68616;-0.00104;,
 -2.84740;4.68644;1.00940;,
 -2.69752;4.83500;1.15068;,
 -2.69712;4.83500;-1.13356;,
 -2.84704;4.68616;-0.98928;,
 -5.14124;0.82296;0.00024;,
 -4.85728;0.82296;1.59232;,
 -4.85728;0.82296;1.59232;,
 -3.29744;4.43500;1.59308;,
 -3.36888;4.43500;-1.57412;,
 -4.92264;0.82296;-1.57412;,
 -4.92264;0.82296;-1.57412;,
 -2.24596;4.43500;1.58468;,
 -2.24596;4.43500;0.00000;,
 -2.24596;4.43500;1.58468;,
 -2.24596;4.43500;-1.57412;,
 -2.24596;4.43500;-1.57412;,
 -1.06344;-4.30868;-2.67932;,
 -0.92336;-4.16860;-2.53924;,
 -0.92336;-4.16904;-1.55852;,
 -1.06388;-4.30868;-1.57412;,
 -0.92336;-4.16860;-2.53924;,
 -1.06344;-4.30868;-2.67932;,
 -1.06532;-3.19848;-2.67932;,
 -0.92336;-3.23092;-2.54116;,
 -1.07616;-0.45704;-2.67932;,
 -0.92336;-0.54332;-2.55324;,
 -1.06344;3.23180;-2.67932;,
 -0.92336;3.17720;-2.53924;,
 -0.92336;0.85032;-2.54060;,
 -1.06476;0.82296;-2.67932;,
 -1.06344;4.43500;-1.57412;,
 -0.92336;4.29492;-1.51256;,
 -0.92336;4.29492;-1.51256;,
 -1.06344;4.43500;-1.57412;,
 -1.06344;4.43500;0.00000;,
 -0.92336;4.29492;0.00000;,
 -1.06344;3.23180;2.67932;,
 -0.92336;3.17720;2.53924;,
 -0.92336;4.29500;1.51260;,
 -1.06336;4.43500;1.57428;,
 -1.06476;0.82296;2.67932;,
 -0.92336;0.85028;2.54060;,
 -1.07612;-0.45704;2.67932;,
 -0.92336;-0.54320;2.55320;,
 -1.06344;-4.30868;2.67932;,
 -0.92336;-4.16860;2.53924;,
 -0.92336;-3.23088;2.54116;,
 -1.06532;-3.19848;2.67932;,
 -0.92336;-4.16860;2.53924;,
 -1.06344;-4.30868;2.67932;,
 -1.06388;-4.30868;1.57480;,
 -0.92336;-4.16904;1.55844;,
 -1.06336;4.43500;1.57428;,
 -0.92336;4.29500;1.51260;,
 -0.92336;1.10416;1.25076;,
 -0.92336;1.10416;0.00000;,
 -0.92336;1.10416;-1.25320;,
 -0.92336;-0.54332;-2.55324;,
 -0.92336;0.85032;-2.54060;,
 -0.92336;3.17720;-2.53924;,
 -0.92336;0.85028;2.54060;,
 -0.92336;3.17720;2.53924;,
 -0.92336;-0.54320;2.55320;,
 -0.92336;-3.23092;-2.54116;,
 -0.92336;-3.23088;2.54116;;
 
 170;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;5,6,2,1;,
 4;6,7,3,2;,
 4;7,4,0,3;,
 4;8,9,5,4;,
 4;10,11,9,8;,
 4;7,6,12,13;,
 4;14,15,16,17;,
 4;10,15,14,11;,
 4;17,16,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,26,29,30;,
 4;31,29,32,33;,
 4;34,32,35,36;,
 4;37,35,38,39;,
 4;40,38,41,42;,
 4;43,41,44,45;,
 4;23,22,46,47;,
 4;47,46,48,49;,
 4;45,44,50,51;,
 4;49,48,51,50;,
 4;52,17,26,25;,
 4;17,19,29,26;,
 4;19,18,32,29;,
 4;32,18,16,35;,
 4;16,15,38,35;,
 4;15,10,41,38;,
 4;11,14,20,23;,
 4;53,54,55,56;,
 4;9,47,6,5;,
 4;49,50,13,12;,
 4;44,8,4,7;,
 4;11,23,47,9;,
 4;57,43,45,46;,
 4;41,10,8,44;,
 4;46,45,51,48;,
 4;44,7,13,50;,
 4;6,47,49,12;,
 4;58,54,53,59;,
 4;60,61,56,55;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;63,78,79,64;,
 4;80,81,82,83;,
 4;84,70,73,85;,
 4;67,86,87,68;,
 4;88,89,75,74;,
 4;90,91,81,80;,
 4;92,62,65,93;,
 4;94,95,96,97;,
 4;98,91,90,94;,
 4;99,100,101,102;,
 4;103,104,105,106;,
 4;83,82,107,108;,
 4;109,110,108,111;,
 4;71,112,113,72;,
 4;65,64,94,97;,
 4;114,115,74,77;,
 4;73,72,111,108;,
 4;116,106,117,118;,
 4;64,79,98,94;,
 4;107,85,73,108;,
 4;115,119,88,74;,
 4;101,116,118,120;,
 4;93,65,97,121;,
 4;102,101,120,122;,
 4;106,105,123,117;,
 4;72,113,124,111;,
 4;78,125,126,79;,
 4;98,79,126,127;,
 4;127,128,91,98;,
 4;81,91,128,129;,
 4;82,81,129,130;,
 4;107,82,130,131;,
 4;131,132,85,107;,
 4;133,84,85,132;,
 4;134,135,136,137;,
 4;125,138,139,126;,
 4;127,126,139,122;,
 4;122,120,128,127;,
 4;129,128,120,118;,
 4;130,129,118,117;,
 4;131,130,117,123;,
 4;140,132,131,123;,
 4;141,133,132,140;,
 4;142,143,144,145;,
 4;143,142,146,147;,
 4;148,149,150,151;,
 4;151,150,152,153;,
 4;154,155,156,157;,
 4;155,154,158,159;,
 4;160,161,162,163;,
 4;161,160,164,165;,
 4;166,167,168,169;,
 4;167,166,170,171;,
 4;172,173,174,175;,
 4;175,174,176,177;,
 4;178,179,180,181;,
 4;179,178,182,183;,
 4;184,185,186,187;,
 4;185,184,188,189;,
 4;190,191,146,142;,
 4;192,193,150,149;,
 4;194,195,158,154;,
 4;196,190,142,145;,
 4;197,198,160,163;,
 4;193,199,152,150;,
 4;198,200,164,160;,
 4;201,194,154,157;,
 4;143,147,170,166;,
 4;148,151,174,173;,
 4;155,159,182,178;,
 4;144,143,166,169;,
 4;162,161,184,187;,
 4;151,153,176,174;,
 4;161,165,188,184;,
 4;156,155,178,181;,
 4;167,171,76,75;,
 4;172,175,77,76;,
 4;179,183,89,88;,
 4;168,167,75,89;,
 4;186,185,115,114;,
 4;175,177,114,77;,
 4;185,189,119,115;,
 4;180,179,88,119;,
 4;202,203,204,205;,
 4;206,207,208,209;,
 4;209,208,210,211;,
 4;212,213,214,215;,
 4;213,212,216,217;,
 4;218,219,220,221;,
 4;222,223,224,225;,
 4;223,222,226,227;,
 4;227,226,228,229;,
 4;230,231,232,233;,
 4;234,235,236,237;,
 4;237,236,205,204;,
 4;215,214,211,210;,
 4;229,228,233,232;,
 4;221,220,238,239;,
 4;239,240,241,221;,
 4;221,241,242,218;,
 4;242,136,243,244;,
 4;218,242,244,245;,
 4;246,240,239,247;,
 4;248,137,240,246;,
 4;243,136,135,249;,
 4;250,134,137,248;,
 4;234,237,134,250;,
 4;204,135,134,237;,
 4;249,135,204,203;,
 4;235,104,103,236;,
 4;233,133,141,230;,
 4;228,84,133,233;,
 4;226,70,84,228;,
 4;222,71,70,226;,
 4;225,112,71,222;,
 4;220,67,66,238;,
 4;219,86,67,220;,
 4;212,62,92,216;,
 4;215,63,62,212;,
 4;210,78,63,215;,
 4;208,125,78,210;,
 4;207,138,125,208;,
 4;205,100,99,202;,
 4;100,205,236,103;;
 
 MeshMaterialList {
  6;
  170;
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  0,
  2,
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
  2,
  0,
  0,
  0,
  0,
  2,
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
   0.800000;0.787200;0.787200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.772000;0.787200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\DX_Framework57\\data\\TEXTURE\\blue.png";
   }
  }
  Material {
   0.800000;0.784000;0.772000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\DX_Framework57\\data\\TEXTURE\\red.png";
   }
  }
  Material {
   0.800000;0.787451;0.774902;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\DX_Framework57\\data\\TEXTURE\\gray.png";
   }
  }
  Material {
   0.800000;0.775200;0.775200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\DX_Framework57\\data\\TEXTURE\\yellow.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\DX_Framework57\\data\\TEXTURE\\black.png";
   }
  }
 }
 MeshNormals {
  180;
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000656;,
  -0.998486;0.055003;0.000272;,
  0.000091;-0.004825;0.999988;,
  1.000000;0.000000;0.000000;,
  0.000000;0.999985;0.005553;,
  0.410038;0.351897;0.841450;,
  0.934274;0.137568;0.328947;,
  0.409095;-0.351930;0.841895;,
  0.933945;-0.137848;0.329765;,
  0.879453;-0.440344;0.180719;,
  0.409088;-0.844169;0.346447;,
  0.879445;-0.439173;-0.183585;,
  0.409074;-0.841902;-0.351938;,
  0.409077;-0.346466;-0.844167;,
  0.933939;-0.135712;-0.330667;,
  0.409539;0.354560;-0.840574;,
  0.934100;0.138751;-0.328946;,
  0.934130;0.139224;-0.328661;,
  0.409470;0.355786;-0.840090;,
  0.410111;0.350645;0.841937;,
  0.934245;0.137081;0.329233;,
  0.187289;-0.003442;0.982299;,
  0.923656;-0.002257;0.383217;,
  0.924085;0.002234;-0.382180;,
  0.187423;0.003425;-0.982273;,
  -0.000042;-0.925614;0.378468;,
  -0.000055;-0.384909;0.922954;,
  0.000126;-0.922242;-0.386613;,
  0.000133;-0.380726;-0.924688;,
  0.000000;-1.000000;0.000000;,
  -0.277815;0.373008;-0.885259;,
  -0.283909;0.375749;0.882161;,
  -0.417083;0.086999;-0.904695;,
  -0.429402;0.089527;0.898665;,
  -0.999803;0.003443;0.019528;,
  -0.180492;0.259380;-0.948760;,
  -0.868622;0.110344;-0.483032;,
  -0.871607;0.111669;0.477317;,
  -0.174269;0.256300;0.950758;,
  0.000000;0.363000;-0.931789;,
  0.001585;0.360879;0.932611;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.847621;-0.530602;,
  0.002131;0.845940;0.533274;,
  -0.419147;0.001559;-0.907917;,
  -0.913880;0.003405;-0.405970;,
  -0.999795;0.003440;0.019934;,
  -0.911488;0.002885;0.411317;,
  -0.431885;0.001377;0.901928;,
  -0.415674;0.001545;-0.909513;,
  -0.914189;0.003405;-0.405273;,
  -0.999779;0.003437;0.020727;,
  -0.911608;0.002883;0.411050;,
  -0.428728;0.001367;0.903433;,
  -0.914467;0.404177;0.019782;,
  -0.900969;0.399056;0.170322;,
  -0.907051;0.400054;-0.131211;,
  0.010161;0.999948;-0.000792;,
  0.012185;0.999895;-0.007890;,
  0.008137;0.999947;0.006307;,
  -0.715110;-0.698879;0.013666;,
  -0.999686;0.016301;0.019023;,
  -0.838534;-0.068555;0.540519;,
  -0.919089;0.187847;0.346395;,
  -0.109412;0.230927;0.966800;,
  -0.449414;0.729083;0.516202;,
  -0.128933;0.238865;-0.962455;,
  -0.461856;0.732189;-0.500588;,
  -0.855983;-0.064510;-0.512964;,
  -0.930132;0.190551;-0.313919;,
  0.923880;0.382683;0.000000;,
  0.382683;0.923880;0.000000;,
  0.023914;0.348781;0.936899;,
  0.012488;0.964220;0.264810;,
  0.011834;0.353475;-0.935369;,
  0.007302;0.964575;-0.263709;,
  -0.689599;0.724077;0.012905;,
  0.021670;0.999765;-0.000536;,
  -0.815472;0.574463;-0.070699;,
  -0.624069;0.717432;-0.309564;,
  -0.287264;0.810459;-0.510525;,
  -0.241104;0.319579;-0.916372;,
  -0.511520;0.767778;0.385829;,
  -0.225073;0.310633;0.923498;,
  -0.813921;0.572410;0.099392;,
  -0.617027;0.714056;0.330761;,
  -0.380900;0.924616;-0.000094;,
  -0.921662;0.387993;-0.000249;,
  -0.196038;0.961973;-0.190202;,
  -0.407059;0.822896;-0.396416;,
  -0.195329;0.961963;0.190980;,
  -0.404526;0.823831;0.397067;,
  0.002133;0.362175;0.932108;,
  0.002172;0.915198;0.402999;,
  0.000000;0.915554;-0.402195;,
  0.000000;0.363000;-0.931789;,
  0.381918;-0.924196;-0.000010;,
  0.923562;-0.383449;-0.000010;,
  0.368364;-0.000131;-0.929682;,
  0.917902;-0.000133;-0.396807;,
  0.405055;0.331553;-0.852058;,
  0.876988;0.174002;-0.447900;,
  0.412212;0.834152;-0.366430;,
  0.935026;0.324638;-0.142605;,
  0.406281;0.331179;0.851620;,
  0.877018;0.173944;0.447864;,
  0.918520;0.000104;0.395374;,
  0.369820;0.000103;0.929103;,
  0.379297;-0.000073;0.925275;,
  0.922472;-0.000074;0.386065;,
  0.381575;-0.924338;0.000005;,
  0.923420;-0.383791;0.000005;,
  0.369789;0.000103;-0.929116;,
  0.918507;0.000104;-0.395404;,
  0.913560;0.000005;0.406704;,
  0.358401;0.000005;0.933568;,
  0.382682;0.923880;-0.000010;,
  0.923879;0.382685;-0.000010;,
  0.935047;0.324592;0.142574;,
  0.413502;0.833559;0.366327;,
  0.923420;-0.383791;-0.000005;,
  0.381575;-0.924338;-0.000005;,
  0.358337;0.000004;-0.933592;,
  0.913532;0.000004;-0.406766;,
  0.917916;-0.000132;0.396775;,
  0.368397;-0.000131;0.929669;,
  0.000148;-0.006613;0.999978;,
  0.000212;-0.011700;0.999932;,
  -0.000097;0.004869;-0.999988;,
  -0.000152;0.006671;-0.999978;,
  -0.000221;0.011818;-0.999930;,
  -0.000079;-0.709629;0.704576;,
  0.000231;-0.705002;-0.709205;,
  0.923744;-0.383006;-0.001817;,
  0.706606;-0.707600;-0.003357;,
  0.000000;0.385820;0.922574;,
  0.000000;-0.385683;0.922632;,
  0.000000;-0.925123;0.379667;,
  0.000000;-0.922630;-0.385687;,
  0.000000;-0.379690;-0.925114;,
  0.000000;0.388649;-0.921386;,
  -0.000332;0.389923;-0.920847;,
  0.000338;0.384524;0.923115;,
  0.000353;-0.003787;0.999993;,
  -0.000349;0.003765;-0.999993;,
  0.000014;0.001263;-0.999999;,
  -0.000022;-0.001248;0.999999;,
  -0.413981;0.001541;-0.910284;,
  -0.914344;0.003410;-0.404924;,
  -0.999771;0.003437;0.021114;,
  -0.911670;0.002882;0.410913;,
  -0.427188;0.001362;0.904162;,
  0.707107;0.707107;0.000000;,
  -0.395619;-0.918384;0.007517;,
  -0.393282;-0.916792;0.069436;,
  0.121430;-0.051207;0.991278;,
  0.092044;-0.038512;-0.995010;,
  -0.396437;-0.916447;-0.054431;,
  0.018671;-0.044851;0.998819;,
  0.008023;-0.035944;-0.999322;,
  -0.909336;0.397501;0.122886;,
  0.397401;0.914895;-0.070997;,
  0.399787;0.916577;-0.007555;,
  -0.132542;0.056670;-0.989556;,
  -0.065945;0.052385;-0.996447;,
  -0.102133;0.043433;0.993822;,
  -0.048134;0.041205;0.997991;,
  0.400561;0.914562;0.055919;,
  -0.999966;0.008286;-0.000574;,
  -0.999971;0.007649;-0.000287;,
  0.000948;0.047863;-0.998853;,
  -0.999975;0.007011;0.000000;,
  0.006006;0.038856;0.999227;,
  0.701908;-0.000136;-0.712268;,
  0.379297;-0.000073;-0.925275;,
  0.705934;-0.708278;0.000018;,
  0.381918;-0.924196;0.000010;;
  170;
  4;1,1,1,1;,
  4;2,2,0,0;,
  4;129,3,130,130;,
  4;4,4,4,4;,
  4;131,132,133,133;,
  4;2,2,2,2;,
  4;0,0,2,2;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;6,7,21,20;,
  4;7,6,8,9;,
  4;9,8,11,10;,
  4;10,11,13,12;,
  4;12,13,14,15;,
  4;15,14,16,17;,
  4;17,16,19,18;,
  4;18,19,25,24;,
  4;20,21,23,22;,
  4;134,26,26,27;,
  4;28,135,29,28;,
  4;136,137,137,136;,
  4;138,139,8,6;,
  4;139,140,11,8;,
  4;140,141,13,11;,
  4;13,141,142,14;,
  4;142,143,16,14;,
  4;143,144,19,16;,
  4;145,138,6,20;,
  4;17,7,9,15;,
  4;146,22,3,129;,
  4;136,136,4,4;,
  4;25,147,132,131;,
  4;145,20,22,146;,
  4;21,18,24,23;,
  4;19,144,147,25;,
  4;26,28,28,26;,
  4;25,131,148,29;,
  4;3,22,27,149;,
  4;21,7,17,18;,
  4;10,12,15,9;,
  4;98,42,42,40;,
  4;96,44,44,46;,
  4;43,95,41,43;,
  4;57,57,58,58;,
  4;42,42,42,42;,
  4;35,49,50,38;,
  4;43,43,43,43;,
  4;44,97,45,44;,
  4;59,59,57,57;,
  4;37,48,49,35;,
  4;97,98,40,45;,
  4;33,37,36,31;,
  4;47,48,37,33;,
  4;30,30,30,30;,
  4;30,30,30,30;,
  4;38,50,51,34;,
  4;39,38,34,32;,
  4;95,96,46,41;,
  4;40,42,33,31;,
  4;61,60,60,61;,
  4;43,41,32,34;,
  4;30,30,30,30;,
  4;42,42,47,33;,
  4;51,43,43,34;,
  4;60,62,62,60;,
  4;30,30,30,30;,
  4;45,40,31,36;,
  4;30,30,30,30;,
  4;30,30,30,30;,
  4;41,46,39,32;,
  4;42,42,42,42;,
  4;47,42,42,52;,
  4;52,53,48,47;,
  4;49,48,53,54;,
  4;50,49,54,55;,
  4;51,50,55,56;,
  4;56,43,43,51;,
  4;43,43,43,43;,
  4;4,4,4,4;,
  4;42,42,42,42;,
  4;52,42,42,150;,
  4;150,151,53,52;,
  4;54,53,151,152;,
  4;55,54,152,153;,
  4;56,55,153,154;,
  4;43,43,56,154;,
  4;43,43,43,43;,
  4;63,64,72,71;,
  4;64,63,65,66;,
  4;66,65,67,68;,
  4;68,67,75,76;,
  4;69,70,78,77;,
  4;70,69,71,72;,
  4;73,74,155,73;,
  4;74,73,73,155;,
  4;79,80,88,87;,
  4;80,79,81,82;,
  4;82,81,83,84;,
  4;84,83,91,92;,
  4;85,86,94,93;,
  4;86,85,87,88;,
  4;89,90,92,91;,
  4;90,89,93,94;,
  4;156,157,157,63;,
  4;158,39,67,158;,
  4;36,159,159,69;,
  4;160,156,63,160;,
  4;4,4,73,73;,
  4;39,161,75,67;,
  4;4,4,73,73;,
  4;162,36,69,77;,
  4;64,66,81,79;,
  4;66,68,163,81;,
  4;70,72,87,85;,
  4;72,64,79,87;,
  4;76,74,89,91;,
  4;68,76,91,83;,
  4;74,78,93,89;,
  4;78,70,85,93;,
  4;80,164,164,165;,
  4;166,84,167,166;,
  4;86,168,168,169;,
  4;170,80,165,170;,
  4;171,90,172,171;,
  4;84,173,173,167;,
  4;90,174,174,172;,
  4;175,86,169,175;,
  4;99,100,123,124;,
  4;176,177,101,102;,
  4;102,101,125,126;,
  4;103,104,116,115;,
  4;104,103,105,106;,
  4;106,105,119,120;,
  4;107,108,121,122;,
  4;108,107,110,109;,
  4;109,110,118,117;,
  4;111,112,127,128;,
  4;178,179,113,114;,
  4;114,113,124,123;,
  4;115,116,126,125;,
  4;117,118,128,127;,
  4;120,119,122,121;,
  4;121,4,4,120;,
  4;120,4,4,106;,
  4;4,4,126,116;,
  4;106,4,116,104;,
  4;109,4,121,108;,
  4;117,4,4,109;,
  4;126,4,4,102;,
  4;127,4,4,117;,
  4;112,114,4,127;,
  4;123,4,4,114;,
  4;102,4,123,100;,
  4;179,30,30,113;,
  4;128,43,43,111;,
  4;118,43,43,128;,
  4;110,43,43,118;,
  4;107,95,43,110;,
  4;122,96,95,107;,
  4;119,44,96,122;,
  4;105,97,44,119;,
  4;103,98,97,105;,
  4;115,42,98,103;,
  4;125,42,42,115;,
  4;101,42,42,125;,
  4;177,42,42,101;,
  4;124,30,30,99;,
  4;30,124,113,30;;
 }
 MeshTextureCoords {
  251;
  0.401620;0.509280;,
  0.401620;0.474090;,
  0.613260;0.474090;,
  0.613260;0.509280;,
  0.401620;0.509280;,
  0.401620;0.474090;,
  0.613260;0.474090;,
  0.613260;0.509280;,
  0.401620;0.509280;,
  0.401620;0.474090;,
  0.401620;0.509280;,
  0.401620;0.474090;,
  0.613260;0.474090;,
  0.613260;0.509280;,
  0.401620;0.438910;,
  0.401620;0.544460;,
  0.401620;0.579650;,
  0.401620;0.685200;,
  0.401620;0.614830;,
  0.401620;0.650020;,
  0.559760;0.438910;,
  0.613260;0.443910;,
  0.613260;0.474090;,
  0.559760;0.474090;,
  0.613260;0.716020;,
  0.559760;0.720390;,
  0.559760;0.685200;,
  0.613260;0.689480;,
  0.613260;0.680920;,
  0.559760;0.650020;,
  0.613260;0.654300;,
  0.613260;0.645740;,
  0.559760;0.614830;,
  0.613260;0.619110;,
  0.613260;0.610550;,
  0.559760;0.579650;,
  0.613260;0.583930;,
  0.613260;0.575370;,
  0.559760;0.544460;,
  0.613260;0.548780;,
  0.613260;0.539440;,
  0.559760;0.509280;,
  0.613260;0.509280;,
  0.613260;0.500890;,
  0.554090;0.509280;,
  0.613260;0.500520;,
  0.613260;0.482890;,
  0.553820;0.474090;,
  0.613260;0.482890;,
  0.613260;0.474090;,
  0.613260;0.509280;,
  0.613260;0.500500;,
  0.401620;0.720390;,
  0.842820;0.263030;,
  0.903600;0.237910;,
  0.913990;0.263000;,
  0.853230;0.288160;,
  0.613260;0.484020;,
  0.871160;0.224560;,
  0.854150;0.231670;,
  0.903560;0.288160;,
  0.878400;0.298580;,
  0.018830;0.526770;,
  0.024550;0.469070;,
  0.054150;0.472190;,
  0.048070;0.530440;,
  0.240050;0.031790;,
  0.193770;0.032490;,
  0.193320;0.002700;,
  0.239840;0.001990;,
  0.373490;0.465850;,
  0.380010;0.523480;,
  0.350890;0.527540;,
  0.343940;0.469390;,
  0.852470;0.515410;,
  0.854830;0.430710;,
  0.884820;0.433900;,
  0.881600;0.520030;,
  0.027790;0.438110;,
  0.057240;0.441120;,
  0.199900;0.486030;,
  0.199500;0.454920;,
  0.248370;0.451210;,
  0.250980;0.482000;,
  0.369770;0.434930;,
  0.340380;0.438390;,
  0.147570;0.033190;,
  0.147120;0.003400;,
  0.823900;0.517370;,
  0.825730;0.431540;,
  0.149200;0.483760;,
  0.151100;0.453010;,
  0.012110;0.569400;,
  0.041880;0.573610;,
  0.105430;0.477990;,
  0.146170;0.489700;,
  0.091860;0.572500;,
  0.070020;0.533130;,
  0.108490;0.446910;,
  0.117350;0.260100;,
  0.150020;0.259070;,
  0.150270;0.318650;,
  0.116790;0.290500;,
  0.244370;0.257420;,
  0.276960;0.258000;,
  0.278020;0.288730;,
  0.245410;0.318290;,
  0.289200;0.444920;,
  0.292770;0.475940;,
  0.309500;0.570190;,
  0.253880;0.488160;,
  0.328980;0.530550;,
  0.387540;0.565760;,
  0.357680;0.570510;,
  0.881800;0.537520;,
  0.851910;0.537220;,
  0.198120;0.315800;,
  0.240630;0.361800;,
  0.198270;0.366630;,
  0.822720;0.536470;,
  0.156890;0.363090;,
  0.074590;0.578100;,
  0.119130;0.349280;,
  0.277060;0.347830;,
  0.327060;0.574920;,
  0.033900;0.371310;,
  0.063280;0.373790;,
  0.114130;0.378680;,
  0.154220;0.387910;,
  0.198590;0.391250;,
  0.243930;0.386560;,
  0.282440;0.377040;,
  0.333220;0.371250;,
  0.362550;0.368260;,
  0.240780;0.231080;,
  0.152780;0.232570;,
  0.159120;0.182640;,
  0.232910;0.181510;,
  0.036150;0.344360;,
  0.065490;0.346720;,
  0.330530;0.344240;,
  0.359840;0.341360;,
  0.855770;0.407700;,
  0.855550;0.412870;,
  0.813020;0.412470;,
  0.808960;0.407090;,
  0.903280;0.410910;,
  0.898850;0.415960;,
  0.905320;0.423530;,
  0.911570;0.421100;,
  0.905750;0.521600;,
  0.899940;0.522100;,
  0.911100;0.552110;,
  0.904450;0.549670;,
  0.799710;0.517410;,
  0.805500;0.518230;,
  0.799670;0.547360;,
  0.792960;0.549630;,
  0.799740;0.416960;,
  0.805840;0.419810;,
  0.851530;0.561420;,
  0.851630;0.555760;,
  0.893880;0.556420;,
  0.897980;0.562150;,
  0.805380;0.560480;,
  0.809750;0.554900;,
  0.855270;0.420150;,
  0.855040;0.425990;,
  0.825470;0.426550;,
  0.821220;0.420330;,
  0.890180;0.423120;,
  0.885490;0.428970;,
  0.889690;0.433420;,
  0.896050;0.429780;,
  0.891610;0.521870;,
  0.886080;0.520960;,
  0.893180;0.543480;,
  0.886220;0.538540;,
  0.813830;0.518500;,
  0.819340;0.518000;,
  0.818250;0.537090;,
  0.811090;0.541570;,
  0.814750;0.426710;,
  0.820890;0.430770;,
  0.851790;0.546970;,
  0.851870;0.541330;,
  0.881600;0.541700;,
  0.885570;0.547450;,
  0.818500;0.546240;,
  0.822760;0.540640;,
  0.856050;0.400600;,
  0.903800;0.403800;,
  0.918560;0.422460;,
  0.912550;0.520680;,
  0.793070;0.516240;,
  0.792680;0.417920;,
  0.809000;0.399970;,
  0.897880;0.568210;,
  0.851420;0.567480;,
  0.917100;0.551200;,
  0.805270;0.566540;,
  0.787000;0.548510;,
  0.117370;0.255220;,
  0.121540;0.249740;,
  0.150510;0.249340;,
  0.150040;0.254680;,
  0.025280;0.346870;,
  0.031360;0.343960;,
  0.029150;0.370890;,
  0.023420;0.369590;,
  0.023320;0.437670;,
  0.017740;0.435020;,
  0.014020;0.526180;,
  0.008360;0.525700;,
  0.013850;0.468690;,
  0.019700;0.468600;,
  0.007170;0.568540;,
  0.001830;0.566690;,
  0.149530;0.043420;,
  0.147640;0.038110;,
  0.193850;0.037420;,
  0.193930;0.042760;,
  0.384800;0.522830;,
  0.390460;0.522240;,
  0.397790;0.562900;,
  0.392490;0.564850;,
  0.378320;0.465300;,
  0.384170;0.465310;,
  0.374240;0.434420;,
  0.379780;0.431670;,
  0.364620;0.340880;,
  0.370750;0.343690;,
  0.373000;0.366370;,
  0.367290;0.367760;,
  0.272560;0.247610;,
  0.276850;0.253070;,
  0.244200;0.253050;,
  0.243550;0.247750;,
  0.240060;0.036710;,
  0.238330;0.042080;,
  0.231840;0.119800;,
  0.195100;0.120360;,
  0.158300;0.120900;,
  0.120690;0.161450;,
  0.120590;0.127590;,
  0.119800;0.071040;,
  0.269810;0.125340;,
  0.268880;0.068780;,
  0.270740;0.159170;,
  0.121590;0.226800;,
  0.271840;0.224530;;
 }
}
