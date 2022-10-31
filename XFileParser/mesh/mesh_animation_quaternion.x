xof 0303txt 0032
// Right hand coordinate system
// Face of polygons are counter clockwise
// Number of meshes 3
// Number of armatures  2
// Mesh Cube2
// Mesh Cube3
// Mesh Cube1

Mesh Cube2 {
  8;
  0.9999999403953552; 4.923368453979492; 0.20311297476291656;,
  0.9999999403953552; 4.923368453979492; -0.2031129002571106;,
  -1.0; 4.923368453979492; -0.2031128704547882;,
  -0.9999995827674866; 4.923368453979492; 0.20311306416988373;,
  1.0000003576278687; -4.923368453979492; 0.20311279594898224;,
  0.9999992847442627; -4.923368453979492; -0.2031131088733673;,
  -1.000000238418579; -4.923368453979492; -0.2031129151582718;,
  -0.9999998807907104; -4.923368453979492; 0.2031129002571106;;
  12;
  3; 1, 3, 0;,
  3; 7, 5, 4;,
  3; 4, 1, 0;,
  3; 5, 2, 1;,
  3; 2, 7, 3;,
  3; 0, 7, 4;,
  3; 1, 2, 3;,
  3; 7, 6, 5;,
  3; 4, 5, 1;,
  3; 5, 6, 2;,
  3; 2, 6, 7;,
  3; 0, 3, 7;;
  MeshNormals {
    12;
    0.0; 1.0; 0.0;,
    -0.0; -1.0; 0.0;,
    1.0; 4.237266892914704e-08; 0.0;,
    -1.4901161193847656e-08; 2.118633624093036e-08; -1.0;,
    -1.0; 3.026617534374054e-08; 1.0270946404489223e-06;,
    5.215405707303944e-08; -1.8159713732757155e-08; 1.0;,
    0.0; 1.0; 0.0;,
    0.0; -1.0; 0.0;,
    1.0; -6.658556372940438e-08; -2.641100991240819e-06;,
    -9.685756907629184e-08; 4.539930653635338e-09; -0.9999999403953552;,
    -1.0; 2.4212949867319367e-08; 8.803672244539484e-07;,
    4.4703490686970326e-08; -1.664640514320581e-08; 0.9999999403953552;;
    12;
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
    3; 11, 11, 11;;
  } // End of MeshNormalsCube2 
  MeshTextureCoords {
    36;
    0.00031246524304151535; 0.3698062300682068;,
    0.36855629086494446; 0.3698062002658844;,
    0.1844344437122345; 0.5539280772209167;,
    0.5539280772209167; 0.7374248504638672;,
    0.814315676689148; 0.4770370423793793;,
    0.8143158555030823; 0.7374247312545776;,
    0.36918121576309204; 0.7374248504638672;,
    0.36918124556541443; 0.3691810965538025;,
    0.5533031821250916; 0.5533030033111572;,
    0.9990624785423279; 0.26070013642311096;,
    0.7386748790740967; 0.00031248852610588074;,
    0.9990625977516174; 0.00031246524304151535;,
    0.00031246524304151535; 0.18505936861038208;,
    0.36855629086494446; 0.18505939841270447;,
    0.18443438410758972; 0.3691812753677368;,
    0.9984377026557922; 0.9984374046325684;,
    0.7380499243736267; 0.7380498051643372;,
    0.9984377026557922; 0.7380497455596924;,
    0.00031246524304151535; 0.1844344437122345;,
    0.18443439900875092; 0.00031246524304151535;,
    0.36855629086494446; 0.1844344437122345;,
    0.7374249696731567; 0.7380497455596924;,
    0.5533031225204468; 0.9221715927124023;,
    0.36918121576309204; 0.7380497455596924;,
    0.5533031821250916; 0.3685561418533325;,
    0.36918121576309204; 0.18443432450294495;,
    0.5533031821250916; 0.00031246524304151535;,
    0.2607000470161438; 0.9996874928474426;,
    0.00031246524304151535; 0.9996874928474426;,
    0.00031249626772478223; 0.7392998337745667;,
    0.5539280772209167; 0.00031246524304151535;,
    0.7380499839782715; 0.1844344288110733;,
    0.5539280772209167; 0.3685562312602997;,
    0.3685562312602997; 0.5545529723167419;,
    0.18443435430526733; 0.7386748790740967;,
    0.00031246524304151535; 0.5545529723167419;;
  } // End of MeshTextureCoords
  MeshMaterialList {
    1;
    12;
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
    0;
    Material Material {
      0.800000011920929; 0.800000011920929; 0.800000011920929; 1.0;;
      50;
      1.0; 1.0; 1.0;;
      0.0; 0.0; 0.0;;
    } // End of Material Material
  } // End of MeshMaterialList

// mesh weights go here

  SkinWeights {
    "Bone2";
    8;
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7;
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0;
    1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.341104507446289e-07, 1.7881392011531716e-07, 1.0, 1.0;;
  } // End of SkinWeights
} // End of mesh Cube2

Mesh Cube3 {
  8;
  0.9999999403953552; 4.923368453979492; 0.20311297476291656;,
  0.9999999403953552; 4.923368453979492; -0.2031129002571106;,
  -1.0; 4.923368453979492; -0.2031128704547882;,
  -0.9999995827674866; 4.923368453979492; 0.20311306416988373;,
  1.0000003576278687; -4.923368453979492; 0.20311279594898224;,
  0.9999992847442627; -4.923368453979492; -0.2031131088733673;,
  -1.000000238418579; -4.923368453979492; -0.2031129151582718;,
  -0.9999998807907104; -4.923368453979492; 0.2031129002571106;;
  12;
  3; 1, 3, 0;,
  3; 7, 5, 4;,
  3; 4, 1, 0;,
  3; 5, 2, 1;,
  3; 2, 7, 3;,
  3; 0, 7, 4;,
  3; 1, 2, 3;,
  3; 7, 6, 5;,
  3; 4, 5, 1;,
  3; 5, 6, 2;,
  3; 2, 6, 7;,
  3; 0, 3, 7;;
  MeshNormals {
    12;
    0.0; 1.0; 0.0;,
    -0.0; -1.0; 0.0;,
    1.0; 4.237266892914704e-08; 0.0;,
    -1.4901161193847656e-08; 2.118633624093036e-08; -1.0;,
    -1.0; 3.026617534374054e-08; 1.0270946404489223e-06;,
    5.215405707303944e-08; -1.8159713732757155e-08; 1.0;,
    0.0; 1.0; 0.0;,
    0.0; -1.0; 0.0;,
    1.0; -6.658556372940438e-08; -2.641100991240819e-06;,
    -9.685756907629184e-08; 4.539930653635338e-09; -0.9999999403953552;,
    -1.0; 2.4212949867319367e-08; 8.803672244539484e-07;,
    4.4703490686970326e-08; -1.664640514320581e-08; 0.9999999403953552;;
    12;
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
    3; 11, 11, 11;;
  } // End of MeshNormalsCube3 
  MeshTextureCoords {
    36;
    0.00031246524304151535; 0.3698062300682068;,
    0.36855629086494446; 0.3698062002658844;,
    0.1844344437122345; 0.5539280772209167;,
    0.5539280772209167; 0.7374248504638672;,
    0.814315676689148; 0.4770370423793793;,
    0.8143158555030823; 0.7374247312545776;,
    0.36918121576309204; 0.7374248504638672;,
    0.36918124556541443; 0.3691810965538025;,
    0.5533031821250916; 0.5533030033111572;,
    0.9990624785423279; 0.26070013642311096;,
    0.7386748790740967; 0.00031248852610588074;,
    0.9990625977516174; 0.00031246524304151535;,
    0.00031246524304151535; 0.18505936861038208;,
    0.36855629086494446; 0.18505939841270447;,
    0.18443438410758972; 0.3691812753677368;,
    0.9984377026557922; 0.9984374046325684;,
    0.7380499243736267; 0.7380498051643372;,
    0.9984377026557922; 0.7380497455596924;,
    0.00031246524304151535; 0.1844344437122345;,
    0.18443439900875092; 0.00031246524304151535;,
    0.36855629086494446; 0.1844344437122345;,
    0.7374249696731567; 0.7380497455596924;,
    0.5533031225204468; 0.9221715927124023;,
    0.36918121576309204; 0.7380497455596924;,
    0.5533031821250916; 0.3685561418533325;,
    0.36918121576309204; 0.18443432450294495;,
    0.5533031821250916; 0.00031246524304151535;,
    0.2607000470161438; 0.9996874928474426;,
    0.00031246524304151535; 0.9996874928474426;,
    0.00031249626772478223; 0.7392998337745667;,
    0.5539280772209167; 0.00031246524304151535;,
    0.7380499839782715; 0.1844344288110733;,
    0.5539280772209167; 0.3685562312602997;,
    0.3685562312602997; 0.5545529723167419;,
    0.18443435430526733; 0.7386748790740967;,
    0.00031246524304151535; 0.5545529723167419;;
  } // End of MeshTextureCoords
  MeshMaterialList {
    1;
    12;
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
    0;
    Material Material {
      0.800000011920929; 0.800000011920929; 0.800000011920929; 1.0;;
      50;
      1.0; 1.0; 1.0;;
      0.0; 0.0; 0.0;;
    } // End of Material Material
  } // End of MeshMaterialList

// mesh weights go here

  SkinWeights {
    "Bone3";
    8;
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7;
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0;
    1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.341104507446289e-07, 1.7881392011531716e-07, 1.0, 1.0;;
  } // End of SkinWeights
} // End of mesh Cube3

Mesh Cube1 {
  8;
  0.9999999403953552; 4.923368453979492; 0.20311297476291656;,
  0.9999999403953552; 4.923368453979492; -0.2031129002571106;,
  -1.0; 4.923368453979492; -0.2031128704547882;,
  -0.9999995827674866; 4.923368453979492; 0.20311306416988373;,
  1.0000003576278687; -4.923368453979492; 0.20311279594898224;,
  0.9999992847442627; -4.923368453979492; -0.2031131088733673;,
  -1.000000238418579; -4.923368453979492; -0.2031129151582718;,
  -0.9999998807907104; -4.923368453979492; 0.2031129002571106;;
  12;
  3; 1, 3, 0;,
  3; 7, 5, 4;,
  3; 4, 1, 0;,
  3; 5, 2, 1;,
  3; 2, 7, 3;,
  3; 0, 7, 4;,
  3; 1, 2, 3;,
  3; 7, 6, 5;,
  3; 4, 5, 1;,
  3; 5, 6, 2;,
  3; 2, 6, 7;,
  3; 0, 3, 7;;
  MeshNormals {
    12;
    0.0; 1.0; 0.0;,
    -0.0; -1.0; 0.0;,
    1.0; 4.237266892914704e-08; 0.0;,
    -1.4901161193847656e-08; 2.118633624093036e-08; -1.0;,
    -1.0; 3.026617534374054e-08; 1.0270946404489223e-06;,
    5.215405707303944e-08; -1.8159713732757155e-08; 1.0;,
    0.0; 1.0; 0.0;,
    0.0; -1.0; 0.0;,
    1.0; -6.658556372940438e-08; -2.641100991240819e-06;,
    -9.685756907629184e-08; 4.539930653635338e-09; -0.9999999403953552;,
    -1.0; 2.4212949867319367e-08; 8.803672244539484e-07;,
    4.4703490686970326e-08; -1.664640514320581e-08; 0.9999999403953552;;
    12;
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
    3; 11, 11, 11;;
  } // End of MeshNormalsCube1 
  MeshTextureCoords {
    36;
    0.00031246524304151535; 0.3698062300682068;,
    0.36855629086494446; 0.3698062002658844;,
    0.1844344437122345; 0.5539280772209167;,
    0.5539280772209167; 0.7374248504638672;,
    0.814315676689148; 0.4770370423793793;,
    0.8143158555030823; 0.7374247312545776;,
    0.36918121576309204; 0.7374248504638672;,
    0.36918124556541443; 0.3691810965538025;,
    0.5533031821250916; 0.5533030033111572;,
    0.9990624785423279; 0.26070013642311096;,
    0.7386748790740967; 0.00031248852610588074;,
    0.9990625977516174; 0.00031246524304151535;,
    0.00031246524304151535; 0.18505936861038208;,
    0.36855629086494446; 0.18505939841270447;,
    0.18443438410758972; 0.3691812753677368;,
    0.9984377026557922; 0.9984374046325684;,
    0.7380499243736267; 0.7380498051643372;,
    0.9984377026557922; 0.7380497455596924;,
    0.00031246524304151535; 0.1844344437122345;,
    0.18443439900875092; 0.00031246524304151535;,
    0.36855629086494446; 0.1844344437122345;,
    0.7374249696731567; 0.7380497455596924;,
    0.5533031225204468; 0.9221715927124023;,
    0.36918121576309204; 0.7380497455596924;,
    0.5533031821250916; 0.3685561418533325;,
    0.36918121576309204; 0.18443432450294495;,
    0.5533031821250916; 0.00031246524304151535;,
    0.2607000470161438; 0.9996874928474426;,
    0.00031246524304151535; 0.9996874928474426;,
    0.00031249626772478223; 0.7392998337745667;,
    0.5539280772209167; 0.00031246524304151535;,
    0.7380499839782715; 0.1844344288110733;,
    0.5539280772209167; 0.3685562312602997;,
    0.3685562312602997; 0.5545529723167419;,
    0.18443435430526733; 0.7386748790740967;,
    0.00031246524304151535; 0.5545529723167419;;
  } // End of MeshTextureCoords
  MeshMaterialList {
    1;
    12;
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
    0;
    Material Material {
      0.800000011920929; 0.800000011920929; 0.800000011920929; 1.0;;
      50;
      1.0; 1.0; 1.0;;
      0.0; 0.0; 0.0;;
    } // End of Material Material
  } // End of MeshMaterialList

// mesh weights go here

  SkinWeights {
    "Bone1";
    8;
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7;
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0;
    1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -5.960464477539063e-08, -1.1920928955078125e-07, 1.0, 1.0;;
  } // End of SkinWeights
} // End of mesh Cube1


Frame Armature2 {
{Cube1}
{Cube2}
{Cube3}
  FrameTransformMatrix { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.1920928955078125e-07, 1.0, 1.0;; }
  Frame Bone1 {
    FrameTransformMatrix { 1.0, 9.685754776000977e-08, 9.685756197086448e-08, 0.0, -9.685754776000977e-08, -1.4901161193847656e-07, 1.0, 0.0, 9.685756197086448e-08, -1.0, -1.1920928955078125e-07, 0.0, 0.0, 0.0, 0.0, 1.0;; }
    Frame Bone2 {
      FrameTransformMatrix { 1.0, 0.0, 2.664533565034481e-15, 0.0, -2.220446049250313e-16, 0.0, 1.0, 0.0, 0.0, -1.0, 2.9802308176840597e-08, 0.0, -1.9371509552001953e-07, -2.980232238769531e-07, 2.0, 1.0;; }
      Frame Bone3 {
        FrameTransformMatrix { 1.0, 0.0, 2.664533565034481e-15, 0.0, -2.220446049250313e-16, 0.0, 1.0, 0.0, 0.0, -1.0, 2.9802308176840597e-08, 0.0, -1.9371509552001953e-07, -2.980232238769531e-07, 4.0, 1.0;; }
      } // End of Bone3
    } // End of Bone2
  } // End of Bone1
} // End of Armature2

Frame Armature1 {
{Armature2}
  FrameTransformMatrix { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, -2.0, 1.0;; }
  Frame Bone {
    FrameTransformMatrix { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0;; }
  } // End of Bone
} // End of Armature1

AnimTicksPerSecond {
24;
} // End of AnimTicksPerSecond

AnimationSet Armature2 {
  Animation Shrink {
    {Bone1}
    AnimationKey {
        0;
        23;
        1; 4; 1.0, 0.0, 0.0, 0.0;;,
        2; 4; 0.9986615777015686, 0.0013726367615163326, -0.0007015394512563944, -0.0026413477025926113;;,
        3; 4; 0.9946178197860718, 0.005519954953342676, -0.002821187721565366, -0.010621979832649231;;,
        4; 4; 0.9878496527671814, 0.012461311183869839, -0.0063688382506370544, -0.023979144170880318;;,
        5; 4; 0.978378176689148, 0.02217516303062439, -0.011333481408655643, -0.04267138987779617;;,
        6; 4; 0.9662744402885437, 0.03458862379193306, -0.01767786219716072, -0.06655845046043396;;,
        7; 4; 0.9516696929931641, 0.04956711083650589, -0.02533320151269436, -0.09538137167692184;;,
        8; 4; 0.93476402759552, 0.06690539419651031, -0.03419460728764534, -0.1287452131509781;;,
        9; 4; 0.9158322215080261, 0.08632174879312515, -0.044118087738752365, -0.16610784828662872;;,
        10; 4; 0.8952243328094482, 0.1074569821357727, -0.05492006987333298, -0.2067781388759613;;,
        11; 4; 0.8733605742454529, 0.12988027930259705, -0.06638036668300629, -0.24992698431015015;;,
        12; 4; 0.8507180213928223, 0.15310220420360565, -0.07824884355068207, -0.2946126461029053;;,
        13; 4; 0.8278116583824158, 0.1765947937965393, -0.09025564789772034, -0.33981919288635254;;,
        14; 4; 0.8051691651344299, 0.1998167335987091, -0.10212411731481552, -0.3845048248767853;;,
        15; 4; 0.7833054065704346, 0.22224004566669464, -0.11358442157506943, -0.4276537001132965;;,
        16; 4; 0.7626974582672119, 0.2433752715587616, -0.12438639998435974, -0.4683239758014679;;,
        17; 4; 0.7437657117843628, 0.26279163360595703, -0.13430988788604736, -0.5056866407394409;;,
        18; 4; 0.7268600463867188, 0.28012987971305847, -0.1431712806224823, -0.5390504598617554;;,
        19; 4; 0.7122553586959839, 0.2951083779335022, -0.1508266031742096, -0.5678732991218567;;,
        20; 4; 0.7001515626907349, 0.3075218200683594, -0.15717101097106934, -0.5917604565620422;;,
        21; 4; 0.6906800866127014, 0.31723570823669434, -0.16213564574718475, -0.6104527115821838;;,
        22; 4; 0.683911919593811, 0.3241770267486572, -0.1656832993030548, -0.623809814453125;;,
        23; 4; 0.6798681020736694, 0.32832443714141846, -0.16780292987823486, -0.6317905187606812;;,
    } // End of AnimationKey
  } // End of Animation Shrink
  Animation Shrink {
    {Bone2}
    AnimationKey {
        0;
        23;
        1; 4; 1.0, 0.0, 0.0, 0.0;;,
        2; 4; 0.9986615777015686, 0.0013726367615163326, -0.0007015394512563944, -0.0026413477025926113;;,
        3; 4; 0.9946178197860718, 0.005519954953342676, -0.002821187721565366, -0.010621979832649231;;,
        4; 4; 0.9878496527671814, 0.012461311183869839, -0.0063688382506370544, -0.023979144170880318;;,
        5; 4; 0.978378176689148, 0.02217516303062439, -0.011333481408655643, -0.04267138987779617;;,
        6; 4; 0.9662744402885437, 0.03458862379193306, -0.01767786219716072, -0.06655845046043396;;,
        7; 4; 0.9516696929931641, 0.04956711083650589, -0.02533320151269436, -0.09538137167692184;;,
        8; 4; 0.93476402759552, 0.06690539419651031, -0.03419460728764534, -0.1287452131509781;;,
        9; 4; 0.9158322215080261, 0.08632174879312515, -0.044118087738752365, -0.16610784828662872;;,
        10; 4; 0.8952243328094482, 0.1074569821357727, -0.05492006987333298, -0.2067781388759613;;,
        11; 4; 0.8733605742454529, 0.12988027930259705, -0.06638036668300629, -0.24992698431015015;;,
        12; 4; 0.8507180213928223, 0.15310220420360565, -0.07824884355068207, -0.2946126461029053;;,
        13; 4; 0.8278116583824158, 0.1765947937965393, -0.09025564789772034, -0.33981919288635254;;,
        14; 4; 0.8051691651344299, 0.1998167335987091, -0.10212411731481552, -0.3845048248767853;;,
        15; 4; 0.7833054065704346, 0.22224004566669464, -0.11358442157506943, -0.4276537001132965;;,
        16; 4; 0.7626974582672119, 0.2433752715587616, -0.12438639998435974, -0.4683239758014679;;,
        17; 4; 0.7437657117843628, 0.26279163360595703, -0.13430988788604736, -0.5056866407394409;;,
        18; 4; 0.7268600463867188, 0.28012987971305847, -0.1431712806224823, -0.5390504598617554;;,
        19; 4; 0.7122553586959839, 0.2951083779335022, -0.1508266031742096, -0.5678732991218567;;,
        20; 4; 0.7001515626907349, 0.3075218200683594, -0.15717101097106934, -0.5917604565620422;;,
        21; 4; 0.6906800866127014, 0.31723570823669434, -0.16213564574718475, -0.6104527115821838;;,
        22; 4; 0.683911919593811, 0.3241770267486572, -0.1656832993030548, -0.623809814453125;;,
        23; 4; 0.6798681020736694, 0.32832443714141846, -0.16780292987823486, -0.6317905187606812;;,
    } // End of AnimationKey
  } // End of Animation Shrink
  Animation Shrink {
    {Bone3}
    AnimationKey {
        0;
        23;
        1; 4; 1.0, 0.0, 0.0, 0.0;;,
        2; 4; 0.9986615777015686, 0.0013726367615163326, -0.0007015394512563944, -0.0026413477025926113;;,
        3; 4; 0.9946178197860718, 0.005519954953342676, -0.002821187721565366, -0.010621979832649231;;,
        4; 4; 0.9878496527671814, 0.012461311183869839, -0.0063688382506370544, -0.023979144170880318;;,
        5; 4; 0.978378176689148, 0.02217516303062439, -0.011333481408655643, -0.04267138987779617;;,
        6; 4; 0.9662744402885437, 0.03458862379193306, -0.01767786219716072, -0.06655845046043396;;,
        7; 4; 0.9516696929931641, 0.04956711083650589, -0.02533320151269436, -0.09538137167692184;;,
        8; 4; 0.93476402759552, 0.06690539419651031, -0.03419460728764534, -0.1287452131509781;;,
        9; 4; 0.9158322215080261, 0.08632174879312515, -0.044118087738752365, -0.16610784828662872;;,
        10; 4; 0.8952243328094482, 0.1074569821357727, -0.05492006987333298, -0.2067781388759613;;,
        11; 4; 0.8733605742454529, 0.12988027930259705, -0.06638036668300629, -0.24992698431015015;;,
        12; 4; 0.8507180213928223, 0.15310220420360565, -0.07824884355068207, -0.2946126461029053;;,
        13; 4; 0.8278116583824158, 0.1765947937965393, -0.09025564789772034, -0.33981919288635254;;,
        14; 4; 0.8051691651344299, 0.1998167335987091, -0.10212411731481552, -0.3845048248767853;;,
        15; 4; 0.7833054065704346, 0.22224004566669464, -0.11358442157506943, -0.4276537001132965;;,
        16; 4; 0.7626974582672119, 0.2433752715587616, -0.12438639998435974, -0.4683239758014679;;,
        17; 4; 0.7437657117843628, 0.26279163360595703, -0.13430988788604736, -0.5056866407394409;;,
        18; 4; 0.7268600463867188, 0.28012987971305847, -0.1431712806224823, -0.5390504598617554;;,
        19; 4; 0.7122553586959839, 0.2951083779335022, -0.1508266031742096, -0.5678732991218567;;,
        20; 4; 0.7001515626907349, 0.3075218200683594, -0.15717101097106934, -0.5917604565620422;;,
        21; 4; 0.6906800866127014, 0.31723570823669434, -0.16213564574718475, -0.6104527115821838;;,
        22; 4; 0.683911919593811, 0.3241770267486572, -0.1656832993030548, -0.623809814453125;;,
        23; 4; 0.6798681020736694, 0.32832443714141846, -0.16780292987823486, -0.6317905187606812;;,
    } // End of AnimationKey
  } // End of Animation Shrink
  Animation Expand {
    {Bone1}
    AnimationKey {
        0;
        25;
        24; 4; 0.6785297393798828, 0.32969698309898376, -0.1685044765472412, -0.634431779384613;;,
        25; 4; 0.679758608341217, 0.3284366726875305, -0.16786034405231476, -0.6320065855979919;;,
        26; 4; 0.6834709644317627, 0.3246293067932129, -0.16591446101665497, -0.6246801614761353;;,
        27; 4; 0.6896854043006897, 0.31825584173202515, -0.16265703737735748, -0.6124157905578613;;,
        28; 4; 0.698387622833252, 0.30933094024658203, -0.15809562802314758, -0.5952417254447937;;,
        29; 4; 0.7095221281051636, 0.29791149497032166, -0.15225926041603088, -0.5732673406600952;;,
        30; 4; 0.7229840755462646, 0.2841050326824188, -0.14520293474197388, -0.5466997623443604;;,
        31; 4; 0.7386119365692139, 0.2680772542953491, -0.13701131939888, -0.5158576965332031;;,
        32; 4; 0.7561818957328796, 0.2500576674938202, -0.12780168652534485, -0.4811827838420868;;,
        33; 4; 0.7754063010215759, 0.23034125566482544, -0.11772486567497253, -0.44324278831481934;;,
        34; 4; 0.7959361672401428, 0.20928601920604706, -0.10696376860141754, -0.40272650122642517;;,
        35; 4; 0.8173694610595703, 0.18730424344539642, -0.09572912752628326, -0.36042720079421997;;,
        36; 4; 0.8392648696899414, 0.16484849154949188, -0.0842522382736206, -0.3172158896923065;;,
        37; 4; 0.8611602783203125, 0.14239273965358734, -0.07277535647153854, -0.27400460839271545;;,
        38; 4; 0.88259357213974, 0.12041094899177551, -0.06154070049524307, -0.23170530796051025;;,
        39; 4; 0.9031234383583069, 0.09935569763183594, -0.05077960342168808, -0.1911889612674713;;,
        40; 4; 0.9223478436470032, 0.07963933050632477, -0.040702782571315765, -0.15324896574020386;;,
        41; 4; 0.939917802810669, 0.06161968410015106, -0.03149314224720001, -0.11857399344444275;;,
        42; 4; 0.9555457830429077, 0.04559198021888733, -0.023301556706428528, -0.08773207664489746;;,
        43; 4; 0.9690077304840088, 0.03178548812866211, -0.016245201230049133, -0.06116443872451782;;,
        44; 4; 0.9801422357559204, 0.020366013050079346, -0.010408848524093628, -0.039190053939819336;;,
        45; 4; 0.9888443946838379, 0.01144111156463623, -0.005847424268722534, -0.022016048431396484;;,
        46; 4; 0.9950588345527649, 0.005067646503448486, -0.0025900304317474365, -0.009751617908477783;;,
        47; 4; 0.9987711906433105, 0.001260221004486084, -0.0006441175937652588, -0.002425074577331543;;,
        48; 4; 1.0, 0.0, 0.0, 0.0;;,
    } // End of AnimationKey
  } // End of Animation Expand
  Animation Expand {
    {Bone2}
    AnimationKey {
        0;
        25;
        24; 4; 0.6785297393798828, 0.32969698309898376, -0.1685044765472412, -0.634431779384613;;,
        25; 4; 0.679758608341217, 0.3284366726875305, -0.16786034405231476, -0.6320065855979919;;,
        26; 4; 0.6834709644317627, 0.3246293067932129, -0.16591446101665497, -0.6246801614761353;;,
        27; 4; 0.6896854043006897, 0.31825584173202515, -0.16265703737735748, -0.6124157905578613;;,
        28; 4; 0.698387622833252, 0.30933094024658203, -0.15809562802314758, -0.5952417254447937;;,
        29; 4; 0.7095221281051636, 0.29791149497032166, -0.15225926041603088, -0.5732673406600952;;,
        30; 4; 0.7229840755462646, 0.2841050326824188, -0.14520293474197388, -0.5466997623443604;;,
        31; 4; 0.7386119365692139, 0.2680772542953491, -0.13701131939888, -0.5158576965332031;;,
        32; 4; 0.7561818957328796, 0.2500576674938202, -0.12780168652534485, -0.4811827838420868;;,
        33; 4; 0.7754063010215759, 0.23034125566482544, -0.11772486567497253, -0.44324278831481934;;,
        34; 4; 0.7959361672401428, 0.20928601920604706, -0.10696376860141754, -0.40272650122642517;;,
        35; 4; 0.8173694610595703, 0.18730424344539642, -0.09572912752628326, -0.36042720079421997;;,
        36; 4; 0.8392648696899414, 0.16484849154949188, -0.0842522382736206, -0.3172158896923065;;,
        37; 4; 0.8611602783203125, 0.14239273965358734, -0.07277535647153854, -0.27400460839271545;;,
        38; 4; 0.88259357213974, 0.12041094899177551, -0.06154070049524307, -0.23170530796051025;;,
        39; 4; 0.9031234383583069, 0.09935569763183594, -0.05077960342168808, -0.1911889612674713;;,
        40; 4; 0.9223478436470032, 0.07963933050632477, -0.040702782571315765, -0.15324896574020386;;,
        41; 4; 0.939917802810669, 0.06161968410015106, -0.03149314224720001, -0.11857399344444275;;,
        42; 4; 0.9555457830429077, 0.04559198021888733, -0.023301556706428528, -0.08773207664489746;;,
        43; 4; 0.9690077304840088, 0.03178548812866211, -0.016245201230049133, -0.06116443872451782;;,
        44; 4; 0.9801422357559204, 0.020366013050079346, -0.010408848524093628, -0.039190053939819336;;,
        45; 4; 0.9888443946838379, 0.01144111156463623, -0.005847424268722534, -0.022016048431396484;;,
        46; 4; 0.9950588345527649, 0.005067646503448486, -0.0025900304317474365, -0.009751617908477783;;,
        47; 4; 0.9987711906433105, 0.001260221004486084, -0.0006441175937652588, -0.002425074577331543;;,
        48; 4; 1.0, 0.0, 0.0, 0.0;;,
    } // End of AnimationKey
  } // End of Animation Expand
  Animation Expand {
    {Bone3}
    AnimationKey {
        0;
        25;
        24; 4; 0.6785297393798828, 0.32969698309898376, -0.1685044765472412, -0.634431779384613;;,
        25; 4; 0.679758608341217, 0.3284366726875305, -0.16786034405231476, -0.6320065855979919;;,
        26; 4; 0.6834709644317627, 0.3246293067932129, -0.16591446101665497, -0.6246801614761353;;,
        27; 4; 0.6896854043006897, 0.31825584173202515, -0.16265703737735748, -0.6124157905578613;;,
        28; 4; 0.698387622833252, 0.30933094024658203, -0.15809562802314758, -0.5952417254447937;;,
        29; 4; 0.7095221281051636, 0.29791149497032166, -0.15225926041603088, -0.5732673406600952;;,
        30; 4; 0.7229840755462646, 0.2841050326824188, -0.14520293474197388, -0.5466997623443604;;,
        31; 4; 0.7386119365692139, 0.2680772542953491, -0.13701131939888, -0.5158576965332031;;,
        32; 4; 0.7561818957328796, 0.2500576674938202, -0.12780168652534485, -0.4811827838420868;;,
        33; 4; 0.7754063010215759, 0.23034125566482544, -0.11772486567497253, -0.44324278831481934;;,
        34; 4; 0.7959361672401428, 0.20928601920604706, -0.10696376860141754, -0.40272650122642517;;,
        35; 4; 0.8173694610595703, 0.18730424344539642, -0.09572912752628326, -0.36042720079421997;;,
        36; 4; 0.8392648696899414, 0.16484849154949188, -0.0842522382736206, -0.3172158896923065;;,
        37; 4; 0.8611602783203125, 0.14239273965358734, -0.07277535647153854, -0.27400460839271545;;,
        38; 4; 0.88259357213974, 0.12041094899177551, -0.06154070049524307, -0.23170530796051025;;,
        39; 4; 0.9031234383583069, 0.09935569763183594, -0.05077960342168808, -0.1911889612674713;;,
        40; 4; 0.9223478436470032, 0.07963933050632477, -0.040702782571315765, -0.15324896574020386;;,
        41; 4; 0.939917802810669, 0.06161968410015106, -0.03149314224720001, -0.11857399344444275;;,
        42; 4; 0.9555457830429077, 0.04559198021888733, -0.023301556706428528, -0.08773207664489746;;,
        43; 4; 0.9690077304840088, 0.03178548812866211, -0.016245201230049133, -0.06116443872451782;;,
        44; 4; 0.9801422357559204, 0.020366013050079346, -0.010408848524093628, -0.039190053939819336;;,
        45; 4; 0.9888443946838379, 0.01144111156463623, -0.005847424268722534, -0.022016048431396484;;,
        46; 4; 0.9950588345527649, 0.005067646503448486, -0.0025900304317474365, -0.009751617908477783;;,
        47; 4; 0.9987711906433105, 0.001260221004486084, -0.0006441175937652588, -0.002425074577331543;;,
        48; 4; 1.0, 0.0, 0.0, 0.0;;,
    } // End of AnimationKey
  } // End of Animation Expand
} // End of AnimationSet Armature2

