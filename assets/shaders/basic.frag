#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D atlasTexture;

uniform vec3 lightDir;    // Направление света (нормализованный вектор)
uniform vec3 lightColor;  // Цвет света

void main()
{
    // Нормаль поверхности (в мировых координатах)
    vec3 norm = normalize(Normal);
    
    // Диффузное освещение (Ламберт)
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Текстура
    vec4 texColor = texture(atlasTexture, TexCoord);

    //FragColor = texColor;
    
    // Для отладки нормалей
    //FragColor = vec4(Normal * 0.5 + 0.5, 1.0);

    // Фоновое освещение
    vec3 ambient = 0.3 * lightColor;
    
    // Итоговый цвет
    vec3 result = (ambient + diffuse) * texColor.rgb;
    FragColor = vec4(result, texColor.a);
}