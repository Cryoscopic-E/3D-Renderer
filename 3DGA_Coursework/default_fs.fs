#version 430 core

out vec4 color;

in VS_OUT
{
  vec2 tc;
  vec3 normals;
  vec3 fragPos;
}fs_in;

uniform mat4 model_matrix;

struct Light{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
  sampler2D textureDiffuse;
  sampler2D textureSpecular;
};

uniform Light light;
uniform vec3 viewPosition;

uniform Material material;

void main(void){
  // ambient
  vec3 ambient=light.ambient*texture(material.textureDiffuse,fs_in.tc).rgb;
  
  // diffuse
  vec3 norm=normalize(fs_in.normals);
  vec3 lightDir=normalize(light.position-fs_in.fragPos);
  float diff=max(dot(norm,lightDir),0.f);
  vec3 diffuse=light.diffuse*diff*texture(material.textureDiffuse,fs_in.tc).rgb;
  
  // specular
  vec3 viewDir=normalize(viewPosition-fs_in.fragPos);
  vec3 reflectDir=reflect(-lightDir,norm);
  float spec=pow(max(dot(viewDir,reflectDir),0.f),material.shininess);
  vec3 specular=light.specular*spec*texture(material.textureSpecular,fs_in.tc).rgb;
  
  vec3 result=ambient+diffuse+specular;
  color=vec4(result,1.f);
}