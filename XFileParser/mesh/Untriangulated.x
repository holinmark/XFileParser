xof 0303txt 0032
// Right hand coordinate system
// Face of polygons are counter clockwise
// Number of meshes 2
// Mesh Cube
// Mesh Cone

Mesh Cube {
  8;
  1.0; 0.9999999403953552; -1.0;,
  1.0; -1.0; -1.0;,
  -1.0000001192092896; -0.9999998211860657; -1.0;,
  -0.9999996423721313; 1.0000003576278687; -1.0;,
  1.0000004768371582; 0.999999463558197; 1.0;,
  0.9999993443489075; -1.0000005960464478; 1.0;,
  -1.0000003576278687; -0.9999996423721313; 1.0;,
  -0.9999999403953552; 1.0; 1.0;;
  6;
  4; 0, 1, 2, 3;,
  4; 4, 7, 6, 5;,
  4; 0, 4, 5, 1;,
  4; 1, 5, 6, 2;,
  4; 2, 6, 7, 3;,
  4; 4, 0, 3, 7;;
  MeshNormals {
    6;
    0.0; 0.0; -1.0;,
    0.0; -0.0; 1.0;,
    1.0; -2.8312206268310547e-07; 4.470341252726939e-08;,
    -2.8312206268310547e-07; -1.0; -1.0430819230577981e-07;,
    -1.0; 2.2351744632942427e-07; -1.341104365337742e-07;,
    2.384185791015625e-07; 1.0; 2.086162567138672e-07;;
    6;
    4; 0, 0, 0, 0;,
    4; 1, 1, 1, 1;,
    4; 2, 2, 2, 2;,
    4; 3, 3, 3, 3;,
    4; 4, 4, 4, 4;,
    4; 5, 5, 5, 5;;
  } // End of MeshNormals
  MeshMaterialList {
    1;
    6;
    0,
    0,
    0,
    0,
    0,
    0;
    Material Material {
      0.800000011920929; 0.800000011920929; 0.800000011920929; 1.0;;
      50;
      1.0; 1.0; 1.0;;
      0.0; 0.0; 0.0;;
    } // End of Material Material
  } // End of MeshMaterialList
  Matrix4x4 { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0; }
} // End of mesh Cube

Mesh Cone {
  33;
  0.0; 1.0; -1.0;,
  0.0; 0.0; 1.0;,
  0.19509032368659973; 0.9807852506637573; -1.0;,
  0.3826834559440613; 0.9238795042037964; -1.0;,
  0.5555702447891235; 0.8314695954322815; -1.0;,
  0.7071067690849304; 0.7071067690849304; -1.0;,
  0.8314696550369263; 0.5555701851844788; -1.0;,
  0.9238795042037964; 0.3826834261417389; -1.0;,
  0.9807852506637573; 0.19509035348892212; -1.0;,
  1.0; 7.549790126404332e-08; -1.0;,
  0.9807853102684021; -0.19509020447731018; -1.0;,
  0.9238795638084412; -0.38268327713012695; -1.0;,
  0.8314696550369263; -0.5555701851844788; -1.0;,
  0.7071067690849304; -0.7071067690849304; -1.0;,
  0.5555701851844788; -0.8314696550369263; -1.0;,
  0.38268327713012695; -0.9238796234130859; -1.0;,
  0.19509008526802063; -0.9807853102684021; -1.0;,
  -3.2584136988589307e-07; -1.0; -1.0;,
  -0.19509072601795197; -0.9807851910591125; -1.0;,
  -0.3826838731765747; -0.9238793253898621; -1.0;,
  -0.5555707216262817; -0.8314692974090576; -1.0;,
  -0.7071072459220886; -0.707106351852417; -1.0;,
  -0.8314700126647949; -0.5555696487426758; -1.0;,
  -0.923879861831665; -0.3826826810836792; -1.0;,
  -0.9807854294776917; -0.1950894594192505; -1.0;,
  -1.0; 9.655991561885457e-07; -1.0;,
  -0.980785071849823; 0.1950913518667221; -1.0;,
  -0.923879086971283; 0.38268446922302246; -1.0;,
  -0.831468939781189; 0.5555712580680847; -1.0;,
  -0.7071058750152588; 0.707107663154602; -1.0;,
  -0.5555691123008728; 0.8314703702926636; -1.0;,
  -0.38268208503723145; 0.9238801002502441; -1.0;,
  -0.19508881866931915; 0.9807855486869812; -1.0;;
  33;
  3; 0, 1, 2;,
  3; 2, 1, 3;,
  3; 3, 1, 4;,
  3; 4, 1, 5;,
  3; 5, 1, 6;,
  3; 6, 1, 7;,
  3; 7, 1, 8;,
  3; 8, 1, 9;,
  3; 9, 1, 10;,
  3; 10, 1, 11;,
  3; 11, 1, 12;,
  3; 12, 1, 13;,
  3; 13, 1, 14;,
  3; 14, 1, 15;,
  3; 15, 1, 16;,
  3; 16, 1, 17;,
  3; 17, 1, 18;,
  3; 18, 1, 19;,
  3; 19, 1, 20;,
  3; 20, 1, 21;,
  3; 21, 1, 22;,
  3; 22, 1, 23;,
  3; 23, 1, 24;,
  3; 24, 1, 25;,
  3; 25, 1, 26;,
  3; 26, 1, 27;,
  3; 27, 1, 28;,
  3; 28, 1, 29;,
  3; 29, 1, 30;,
  3; 30, 1, 31;,
  3; 31, 1, 32;,
  3; 32, 1, 0;,
  32; 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32;;
  MeshNormals {
    33;
    0.08775367587804794; 0.8909766674041748; 0.4454883337020874;,
    0.25988826155662537; 0.8567368984222412; 0.4454883337020874;,
    0.42203569412231445; 0.7895733118057251; 0.4454883635044098;,
    0.5679645538330078; 0.6920667290687561; 0.445488303899765;,
    0.6920667886734009; 0.5679646134376526; 0.4454883635044098;,
    0.7895733714103699; 0.4220355451107025; 0.4454883635044098;,
    0.8567368984222412; 0.25988835096359253; 0.4454883635044098;,
    0.89097660779953; 0.08775369077920914; 0.445488303899765;,
    0.89097660779953; -0.08775342255830765; 0.445488303899765;,
    0.8567368984222412; -0.25988835096359253; 0.4454883635044098;,
    0.7895734310150146; -0.42203548550605774; 0.4454882740974426;,
    0.6920667886734009; -0.5679646134376526; 0.4454883635044098;,
    0.5679646134376526; -0.6920667886734009; 0.4454883635044098;,
    0.42203569412231445; -0.7895733118057251; 0.4454883635044098;,
    0.2598879635334015; -0.8567370772361755; 0.4454883933067322;,
    0.08775337040424347; -0.8909766674041748; 0.445488303899765;,
    -0.08775391429662704; -0.89097660779953; 0.4454883337020874;,
    -0.25988876819610596; -0.8567368388175964; 0.4454883635044098;,
    -0.4220360219478607; -0.7895731329917908; 0.44548842310905457;,
    -0.5679649114608765; -0.692066490650177; 0.445488303899765;,
    -0.6920672655105591; -0.5679640173912048; 0.4454883337020874;,
    -0.789573609828949; -0.4220351576805115; 0.4454883635044098;,
    -0.8567372560501099; -0.2598874270915985; 0.4454883337020874;,
    -0.8909767866134644; -0.0877528190612793; 0.4454883337020874;,
    -0.8909765481948853; 0.08775445818901062; 0.445488303899765;,
    -0.8567366600036621; 0.25988930463790894; 0.4454883635044098;,
    -0.7895728945732117; 0.4220365583896637; 0.44548842310905457;,
    -0.692065954208374; 0.5679654479026794; 0.4454883635044098;,
    -0.567963719367981; 0.6920674443244934; 0.4454883337020874;,
    -0.4220346212387085; 0.7895739078521729; 0.4454883635044098;,
    -0.25988689064979553; 0.8567374348640442; 0.4454883337020874;,
    -0.08775299042463303; 0.8909766674041748; 0.4454883337020874;,
    5.7285621579694634e-08; 0.0; -1.0;;
    33;
    3; 0, 0, 0;,
    3; 1, 1, 1;,
    3; 2, 2, 2;,
    3; 3, 3, 3;,
    3; 4, 4, 4;,
    3; 5, 5, 5;,
    3; 6, 6, 6;,
    3; 7, 7, 7;,
    3; 8, 8, 8;,
    3; 9, 9, 9;,
    3; 10, 10, 10;,
    3; 11, 11, 11;,
    3; 12, 12, 12;,
    3; 13, 13, 13;,
    3; 14, 14, 14;,
    3; 15, 15, 15;,
    3; 16, 16, 16;,
    3; 17, 17, 17;,
    3; 18, 18, 18;,
    3; 19, 19, 19;,
    3; 20, 20, 20;,
    3; 21, 21, 21;,
    3; 22, 22, 22;,
    3; 23, 23, 23;,
    3; 24, 24, 24;,
    3; 25, 25, 25;,
    3; 26, 26, 26;,
    3; 27, 27, 27;,
    3; 28, 28, 28;,
    3; 29, 29, 29;,
    3; 30, 30, 30;,
    3; 31, 31, 31;,
    32; 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32;;
  } // End of MeshNormals
  Matrix4x4 { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -2.719332456588745, 0.0, 1.0; }
} // End of mesh Cone
