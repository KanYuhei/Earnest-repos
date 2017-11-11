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
 82;
 -0.00083;0.00290;0.00233;,
 -0.32765;0.00290;0.34246;,
 -0.00083;0.00290;0.48334;,
 -0.46303;0.00290;0.00234;,
 -0.32765;0.00290;-0.33779;,
 -0.00084;0.00290;-0.47867;,
 0.32598;0.00290;-0.33779;,
 0.46136;0.00290;0.00233;,
 0.32598;0.00290;0.34245;,
 -0.35892;-0.00340;0.37500;,
 -0.00083;-0.00340;0.52936;,
 -0.50725;-0.00340;0.00234;,
 -0.35892;-0.00340;-0.37033;,
 -0.00084;-0.00340;-0.52469;,
 0.35725;-0.00340;-0.37033;,
 0.50557;-0.00340;0.00233;,
 0.35725;-0.00340;0.37500;,
 -0.38543;-0.02133;0.40258;,
 -0.00083;-0.02133;0.56837;,
 -0.54473;-0.02133;0.00234;,
 -0.38543;-0.02133;-0.39791;,
 -0.00084;-0.02133;-0.56370;,
 0.38375;-0.02133;-0.39791;,
 0.54306;-0.02133;0.00233;,
 0.38376;-0.02133;0.40258;,
 -0.40314;-0.04817;0.42102;,
 -0.00083;-0.04817;0.59444;,
 -0.56978;-0.04817;0.00234;,
 -0.40314;-0.04817;-0.41635;,
 -0.00084;-0.04817;-0.58977;,
 0.40147;-0.04817;-0.41635;,
 0.56811;-0.04817;0.00233;,
 0.40147;-0.04817;0.42101;,
 -0.40936;-0.07982;0.42749;,
 -0.00083;-0.07982;0.60359;,
 -0.57857;-0.07982;0.00234;,
 -0.40936;-0.07982;-0.42282;,
 -0.00084;-0.07982;-0.59892;,
 0.40769;-0.07982;-0.42282;,
 0.57690;-0.07982;0.00233;,
 0.40769;-0.07982;0.42749;,
 -0.58586;-0.91752;0.53182;,
 -0.00083;-0.91752;0.75114;,
 -0.82819;-0.91752;0.00234;,
 -0.58586;-0.91752;-0.52715;,
 -0.00084;-0.91752;-0.74647;,
 0.58419;-0.91752;-0.52715;,
 0.82651;-0.91752;0.00233;,
 0.58419;-0.91752;0.53181;,
 -0.57695;-0.95466;0.52376;,
 -0.00083;-0.95466;0.73974;,
 -0.81559;-0.95466;0.00234;,
 -0.57695;-0.95466;-0.51909;,
 -0.00084;-0.95466;-0.73507;,
 0.57528;-0.95466;-0.51909;,
 0.81392;-0.95466;0.00233;,
 0.57528;-0.95466;0.52375;,
 -0.55159;-0.98614;0.50080;,
 -0.00083;-0.98614;0.70727;,
 -0.77972;-0.98614;0.00234;,
 -0.55159;-0.98614;-0.49613;,
 -0.00084;-0.98614;-0.70261;,
 0.54992;-0.98614;-0.49613;,
 0.77805;-0.98614;0.00233;,
 0.54992;-0.98614;0.50080;,
 -0.51363;-1.00717;0.46645;,
 -0.00083;-1.00717;0.65869;,
 -0.72604;-1.00717;0.00234;,
 -0.51363;-1.00717;-0.46178;,
 -0.00084;-1.00717;-0.65402;,
 0.51196;-1.00717;-0.46178;,
 0.72436;-1.00717;0.00233;,
 0.51196;-1.00717;0.46644;,
 -0.46885;-1.01456;0.42592;,
 -0.00083;-1.01456;0.60138;,
 -0.66271;-1.01456;0.00234;,
 -0.46886;-1.01456;-0.42125;,
 -0.00084;-1.01456;-0.59671;,
 0.46718;-1.01456;-0.42125;,
 0.66104;-1.01456;0.00233;,
 0.46718;-1.01456;0.42592;,
 -0.00083;-1.01456;0.00233;;
 
 88;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,7,6;,
 3;0,8,7;,
 3;0,2,8;,
 4;2,1,9,10;,
 4;1,3,11,9;,
 4;3,4,12,11;,
 4;4,5,13,12;,
 4;5,6,14,13;,
 4;6,7,15,14;,
 4;7,8,16,15;,
 4;8,2,10,16;,
 4;10,9,17,18;,
 4;9,11,19,17;,
 4;11,12,20,19;,
 4;12,13,21,20;,
 4;13,14,22,21;,
 4;14,15,23,22;,
 4;15,16,24,23;,
 4;16,10,18,24;,
 4;18,17,25,26;,
 4;17,19,27,25;,
 4;19,20,28,27;,
 4;20,21,29,28;,
 4;21,22,30,29;,
 4;22,23,31,30;,
 4;23,24,32,31;,
 4;24,18,26,32;,
 4;26,25,33,34;,
 4;25,27,35,33;,
 4;27,28,36,35;,
 4;28,29,37,36;,
 4;29,30,38,37;,
 4;30,31,39,38;,
 4;31,32,40,39;,
 4;32,26,34,40;,
 4;34,33,41,42;,
 4;33,35,43,41;,
 4;35,36,44,43;,
 4;36,37,45,44;,
 4;37,38,46,45;,
 4;38,39,47,46;,
 4;39,40,48,47;,
 4;40,34,42,48;,
 4;42,41,49,50;,
 4;41,43,51,49;,
 4;43,44,52,51;,
 4;44,45,53,52;,
 4;45,46,54,53;,
 4;46,47,55,54;,
 4;47,48,56,55;,
 4;48,42,50,56;,
 4;50,49,57,58;,
 4;49,51,59,57;,
 4;51,52,60,59;,
 4;52,53,61,60;,
 4;53,54,62,61;,
 4;54,55,63,62;,
 4;55,56,64,63;,
 4;56,50,58,64;,
 4;58,57,65,66;,
 4;57,59,67,65;,
 4;59,60,68,67;,
 4;60,61,69,68;,
 4;61,62,70,69;,
 4;62,63,71,70;,
 4;63,64,72,71;,
 4;64,58,66,72;,
 4;66,65,73,74;,
 4;65,67,75,73;,
 4;67,68,76,75;,
 4;68,69,77,76;,
 4;69,70,78,77;,
 4;70,71,79,78;,
 4;71,72,80,79;,
 4;72,66,74,80;,
 3;74,73,81;,
 3;73,75,81;,
 3;75,76,81;,
 3;76,77,81;,
 3;77,78,81;,
 3;78,79,81;,
 3;79,80,81;,
 3;80,74,81;;
 
 MeshMaterialList {
  13;
  88;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\3131372i.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\leather-texture-thumbnail2.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile2\\data\\TEXTURE\\iron2.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\160304_10.png";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\images (1).jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\p0447_m.jpg";
   }
  }
  Material {
   0.449400;0.449400;0.449400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.299600;0.299600;0.299600;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\771.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\eye.png";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\images.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\razer.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\iron.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\gold.jpg";
   }
  }
  Material {
   0.600000;0.600000;0.600000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.400000;0.400000;0.400000;;
   TextureFilename {
    "C:\\Users\\yamashita.shun\\Desktop\\Compile用\\Compile\\data\\TEXTURE\\ruby.jpg";
   }
  }
 }
 MeshNormals {
  82;
  0.000000;1.000000;0.000000;,
  -0.000000;0.997693;0.067884;,
  -0.049937;0.997598;0.047996;,
  -0.070613;0.997504;0.000000;,
  -0.049937;0.997599;-0.047995;,
  -0.000000;0.997693;-0.067883;,
  0.049935;0.997599;-0.047995;,
  0.070612;0.997504;0.000000;,
  0.049937;0.997598;0.047995;,
  0.000001;0.960398;0.278630;,
  -0.204386;0.958912;0.196759;,
  -0.288660;0.957431;0.000001;,
  -0.204385;0.958913;-0.196754;,
  0.000001;0.960400;-0.278625;,
  0.204389;0.958911;-0.196760;,
  0.288667;0.957430;-0.000001;,
  0.204390;0.958911;0.196760;,
  0.000003;0.818137;0.575024;,
  -0.418964;0.812878;0.404598;,
  -0.589607;0.807690;0.000002;,
  -0.418964;0.812878;-0.404596;,
  0.000001;0.818135;-0.575027;,
  0.418966;0.812875;-0.404601;,
  0.589610;0.807688;-0.000000;,
  0.418969;0.812874;0.404600;,
  0.000002;0.505834;0.862631;,
  -0.622245;0.498802;0.603331;,
  -0.870600;0.491991;0.000003;,
  -0.622248;0.498794;-0.603334;,
  -0.000002;0.505813;-0.862643;,
  0.622244;0.498799;-0.603334;,
  0.870598;0.491995;0.000000;,
  0.622245;0.498808;0.603326;,
  0.000002;0.232303;0.972643;,
  -0.684754;0.250219;0.684472;,
  -0.963378;0.268148;0.000002;,
  -0.684756;0.250212;-0.684473;,
  -0.000002;0.232286;-0.972648;,
  0.684755;0.250213;-0.684474;,
  0.963378;0.268146;-0.000001;,
  0.684757;0.250225;0.684467;,
  0.000004;-0.056568;0.998399;,
  -0.688550;-0.041649;0.723992;,
  -0.999653;-0.026329;0.000001;,
  -0.688550;-0.041647;-0.723992;,
  -0.000001;-0.056563;-0.998399;,
  0.688550;-0.041643;-0.723992;,
  0.999653;-0.026328;-0.000001;,
  0.688555;-0.041646;0.723988;,
  0.000004;-0.525099;0.851041;,
  -0.570630;-0.541919;0.617014;,
  -0.828473;-0.560029;0.000000;,
  -0.570632;-0.541911;-0.617018;,
  -0.000001;-0.525086;-0.851049;,
  0.570636;-0.541904;-0.617020;,
  0.828480;-0.560019;-0.000001;,
  0.570638;-0.541909;0.617014;,
  0.000002;-0.832429;0.554131;,
  -0.362303;-0.843791;0.395921;,
  -0.517819;-0.855490;0.000000;,
  -0.362305;-0.843789;-0.395923;,
  -0.000001;-0.832426;-0.554136;,
  0.362308;-0.843787;-0.395924;,
  0.517826;-0.855486;-0.000001;,
  0.362309;-0.843789;0.395920;,
  0.000001;-0.964384;0.264506;,
  -0.170371;-0.967370;0.187532;,
  -0.241582;-0.970380;0.000000;,
  -0.170371;-0.967371;-0.187530;,
  -0.000000;-0.964384;-0.264505;,
  0.170372;-0.967370;-0.187531;,
  0.241584;-0.970380;-0.000000;,
  0.170373;-0.967371;0.187530;,
  0.000001;-0.997950;0.063995;,
  -0.040985;-0.998134;0.045261;,
  -0.057976;-0.998318;-0.000000;,
  -0.040986;-0.998134;-0.045263;,
  0.000000;-0.997950;-0.063997;,
  0.040987;-0.998134;-0.045264;,
  0.057978;-0.998318;-0.000000;,
  0.040987;-0.998134;0.045263;,
  0.000000;-1.000000;-0.000000;;
  88;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,8,16,15;,
  4;8,1,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,17,25,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,25,33,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,33,41,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,41,49,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,49,57,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,57,65,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,65,73,80;,
  3;73,74,81;,
  3;74,75,81;,
  3;75,76,81;,
  3;76,77,81;,
  3;77,78,81;,
  3;78,79,81;,
  3;79,80,81;,
  3;80,73,81;;
 }
 MeshTextureCoords {
  82;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
