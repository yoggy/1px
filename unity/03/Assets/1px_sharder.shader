Shader "Custom/1px_sharder" {
    SubShader {
        Pass {
			CGPROGRAM
            #pragma vertex vert_img
            #pragma fragment frag

            #include "UnityCG.cginc"

            fixed4 frag(v2f_img i) : SV_Target {
            	int n = (int)(_Time.y * 4) % 4;
            	float4 c;
            	switch(n) {
            	case 0:
            		c = float4(1.0, 0.0, 0.0, 1.0);
            		break;
            	case 1:
            		c = float4(0.0, 1.0, 0.0, 1.0);
            		break;
            	case 2:
            		c = float4(0.0, 0.0, 1.0, 1.0);
            		break;
            	case 3:
            		c = float4(0.0, 0.0, 0.0, 1.0);
            		break;
            	}
            	
            	float dx = i.uv.x - 0.5;
            	float dy = i.uv.y - 0.5;
            	float d = sqrt(dx * dx + dy * dy);

            	if (d > 0.002) return float4(0.0, 0.0, 0.0, 1.0); 
            
                return c;
            }
            ENDCG
    	}
    } 
	FallBack "Diffuse"
}
