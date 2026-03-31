#version 430 core                  
			                       
layout (location = 0) in vec4 color;       
layout (location = 3) in float time; 
out vec4 vs_color; 
void main(void)                    
{                                  
	const vec4 vertices[6] = vec4[6](    
	vec4(-0.03, -0.8, 0.5, 1.0),  
	vec4( 0.03, -0.8, 0.5, 1.0),  
	vec4(-0.03,  0.0, 0.5, 1.0),  
	
	vec4( 0.03, -0.8, 0.5, 1.0),  
	vec4( 0.03,  0.0, 0.5, 1.0),  
	vec4(-0.03,  0.0, 0.5, 1.0)); 
	
	// 쉐이더에서 행렬을 정의할 때 열단위로 한 줄씩 정의한다
	
	// 막대기 이동행렬
	mat4 trans_mat;
	trans_mat[0]=vec4(1,0,0,0);
	trans_mat[1]=vec4(0,1,0,0);
	trans_mat[2]=vec4(0,0,1,0);
	trans_mat[3]=vec4(sin(time),0,0,1);
	
	// 모델 메트릭스 선언
	mat4 model_mat;
	model_mat = trans_mat; 
	
	
	// 뷰 메트릭스 선언
	vec3 eyePos = vec3(0.2,0.3,0.8);
	vec3 target = vec3(0.0,0.0,0.5);
	vec3 upVec = vec3(0.0,1.0,0.0);
	
	vec3 D = normalize(eyePos-target);
	vec3 R = normalize(cross(upVec, D));
	vec3 U = cross(D, R);
	
	mat4 view_mat1;
	view_mat1[0]=vec4(R.x, U.x, D.x, 0);
	view_mat1[1]=vec4(R.y, U.y, D.y, 0);
	view_mat1[2]=vec4(R.z, U.z, D.z, 0);
	view_mat1[3]=vec4(0, 0, 0, 1);
	
	mat4 view_mat2;
	view_mat2[0]=vec4(1,0,0,0);
	view_mat2[1]=vec4(0,1,0,0);
	view_mat2[2]=vec4(0,0,1,0);
	view_mat2[3]=vec4(-eyePos.x, -eyePos.y, -eyePos.z, 1);
	
	mat4 view_mat;
	view_mat = view_mat1 * view_mat2;
	
	
	// 프로젝션 메트릭스 선언
	float left=-0.1; 
	float right=0.1; 
	float top=0.07;
	float bottom=-0.07;
	float near=0.1;
	float far=10;
	
	mat4 proj_mat;
	proj_mat[0]=vec4(2*near/(right-left),0,0,0);
	proj_mat[1]=vec4(0,2*near/(top-bottom),0,0);
	proj_mat[2]=vec4((right+left)/(right-left),(top+bottom)/(top-bottom),(near+far)/(near-far),-1.0);
	proj_mat[3]=vec4(0,0,2*near*far/(near-far),0);
	
	gl_Position = proj_mat * view_mat * model_mat * vertices[gl_VertexID];                            
	// Scaling -> Rotation -> Translation 순서로 계산해야 함.
}															 