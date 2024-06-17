#version 330 core

layout(location = 0) in vec3 position;  
layout(location = 1) in vec3 normal;

out vec3 vertPosition;                  //passing argument to fragment shader
out vec3 vertNormal;                    //passing normal to fragment shader

 uniform mat4 uniProjection;
 uniform mat4 uniView;
uniform mat4 uniModel;

void main()
{
          vertPosition = vec3(uniProjection * uniView * uniModel * vec4(position,1.0));
          vertNormal = normalize(transpose(inverse(mat3(uniModel))) * normal);
          gl_Position = uniProjection * uniView * uniModel * vec4(position,1.0);
}