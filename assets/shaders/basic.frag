#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D atlasTexture;

uniform vec3 lightDir;    // ����������� ����� (��������������� ������)
uniform vec3 lightColor;  // ���� �����

void main()
{
    // ������� ����������� (� ������� �����������)
    vec3 norm = normalize(Normal);
    
    // ��������� ��������� (�������)
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // ��������
    vec4 texColor = texture(atlasTexture, TexCoord);

    //FragColor = texColor;
    
    // ��� ������� ��������
    //FragColor = vec4(Normal * 0.5 + 0.5, 1.0);

    // ������� ���������
    vec3 ambient = 0.3 * lightColor;
    
    // �������� ����
    vec3 result = (ambient + diffuse) * texColor.rgb;
    FragColor = vec4(result, texColor.a);
}