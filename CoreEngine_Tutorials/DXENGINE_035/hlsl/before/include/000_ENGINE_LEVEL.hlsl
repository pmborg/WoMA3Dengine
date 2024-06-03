// DX11: Use All
//#if ENGINE_LEVEL >= 23
//#pragma message( "SELECT: PS_USE_LIGHT" )  
#define PS_USE_LIGHT			//23
//#endif
//#if ENGINE_LEVEL >= 26
//#pragma message( "SELECT: PS_USE_ALFACOLOR" )  
#define PS_USE_ALFACOLOR		//26
//#endif
//#if ENGINE_LEVEL >= 28
//#pragma message( "SELECT: PS_USE_FONT" )  
//#define PS_USE_FONT				//28
//#endif
//#if ENGINE_LEVEL >= 30
//#pragma message( "SELECT: PS_USE_SKY" )  
//#define PS_USE_SKY				//30
//#endif
//#if ENGINE_LEVEL >= 31
//#pragma message( "SELECT: PS_USE_FOG" )  
//#define PS_USE_FOG				//31
//#endif
//#if ENGINE_LEVEL >= 34
//#pragma message( "SELECT: PS_USE_FADE" )  
//#define PS_USE_FADE				//34
//#endif
//#if ENGINE_LEVEL >= 43
//#pragma message( "SELECT: PS_USE_ALFA_TEXTURE" )  
#define PS_USE_ALFA_TEXTURE		//43
//#endif
//#if ENGINE_LEVEL >= 44
//#pragma message( "SELECT: PS_USE_SPECULAR" )  
#define PS_USE_SPECULAR			//44
//#endif
//#if ENGINE_LEVEL >= 45
//#pragma message( "SELECT: PS_USE_SHADOWMAP_TEXTURE" )  
//#define PS_USE_SHADOWMAP_TEXTURE//45
//#endif

float4  fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

//define GS_USE

/*
    PS 1.0 — Unreleased 3dfx Rampage, DirectX 8.
    PS 1.1 — GeForce 3, DirectX 8.
    PS 1.2 — 3Dlabs Wildcat VP, DirectX 8.0a.
    PS 1.3 — GeForce 4 Ti, DirectX 8.0a.
    PS 1.4 — Radeon 8500-9250, Matrox Parhelia, DirectX 8.1.
    Shader Model 2.0 — Radeon 9500-9800/X300-X600, DirectX 9.
    Shader Model 2.0a — GeForce FX/PCX-optimized model, DirectX 9.0a.
    Shader Model 2.0b — Radeon X700-X850 shader model, DirectX 9.0b.
    Shader Model 3.0 — Radeon X1000 and GeForce 6, DirectX 9.0c.
    Shader Model 4.0 — Radeon HD 2000 and GeForce 8, DirectX 10.
    Shader Model 4.1 — Radeon HD 3000 and GeForce 200, DirectX 10.1.
    Shader Model 5.0 — Radeon HD 5000 and GeForce 400, DirectX 11.
    Shader Model 5.1 — GCN 1+, Fermi+, DirectX 12 (11_0+) with WDDM 2.0.
    Shader Model 6.0 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.1.
    Shader Model 6.1 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.3.
    Shader Model 6.2 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.4.
    Shader Model 6.3 — GCN 1+, Kepler+, DirectX 12 (11_0+) with WDDM 2.5.
    Shader Model 6.4 — GCN 1+, Kepler+, Skylake+, DirectX 12 (11_0+) with WDDM 2.6.
    Shader Model 6.5 — GCN 1+, Kepler+, Skylake+, DirectX 12 (11_0+) with WDDM 2.7.
    Shader Model 6.6 — GCN 4+, Maxwell+, DirectX 12 (11_0+) with WDDM 3.0.
    Shader Model 6.7 — GCN 4+, Maxwell+, DirectX 12 (12_0+) with WDDM 3.1.
*/