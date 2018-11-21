#version 400

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 texCoord;

out vec4 color;
out vec2 tex;
out float lighting;
out float ambience;

uniform mat4 uModelViewMatrix;
uniform mat4 ViewMatrix;
uniform mat3 uNormalMatrix;

uniform int lampLighting;
uniform int wallLighting;

void main() {
  gl_Position = uModelViewMatrix * vPosition;

  // Object properties
  vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0); 
  vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0); //
  vec4 specular = vec4(0.55, 0.55, 0.55, 1.0);
  float shininess = 0.5;

  vec3 normal = normalize(uNormalMatrix*normalize(vNormal));

  vec4 lightPos[2], spec[2];
  vec3 lightDir[2];
  float dotProduct[2], intensity[2];

  // Defining Light 
  lightPos[0] = vec4(0.0, 0.0, 95.0, 1.0);
  lightDir[0] = vec3(ViewMatrix * lightPos[0]); 
  lightDir[0] = lightDir[0] - vec3(gl_Position);
  lightDir[0] = normalize(lightDir[0]);  

  dotProduct[0] = dot(normal, lightDir[0]);
  intensity[0] =  max(dotProduct[0], 0.0);

  // Compute specular component only if light falls on vertex
  if(intensity[0] > 0.0)
  {
    vec3 eye = normalize( vec3(-gl_Position));
    vec3 h = normalize(lightDir[0] + eye );
    float intSpec = max(dot(h,normal), 0.0);  
    spec[0] = specular * pow(intSpec, shininess);
  }


  // Defining Light 
  lightPos[1] = vec4(-70.0, 60.0, 20.0, 1.0);
  lightDir[1] = vec3(ViewMatrix * lightPos[1]); 
  lightDir[1] = lightDir[1] - vec3(gl_Position);
  lightDir[1] = normalize(lightDir[1]);  

  dotProduct[1] = dot(normal, lightDir[1]);
  intensity[1] =  max(dotProduct[1], 0.0);

  // Compute specular component only if light falls on vertex
  if(intensity[1] > 0.0)
  {
    vec3 eye = normalize( vec3(-gl_Position));
    vec3 h = normalize(lightDir[1] + eye );
    float intSpec = max(dot(h,normal), 0.0);  
    spec[1] = specular * pow(intSpec, shininess);
  }

  if(wallLighting == 0){
    intensity[1] = 0; spec[1] = vec4(0.0);
  }
  if(lampLighting == 0){
    intensity[0] = 0; spec[0] = vec4(0.0);
  }

  color = max(((intensity[0] + intensity[1])*diffuse+spec[0]+spec[1])*vColor, ambient); //
  tex = texCoord;
    
  lighting = (intensity[0] + intensity[1]);
  ambience = ambient.x;

  tex = texCoord;
}
