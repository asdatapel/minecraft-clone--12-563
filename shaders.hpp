#pragma once
const char* vertexShaderSource = "\n#version 150 \n\nin vec3 texcoord; \nin vec3 position; \nin vec3 incolor; \nin float sunlightlevel; \nin float aolevel;\n\nout vec3 Color; \nout vec3 texCoord; \nout float sunlightLevel; \nout float aoLevel;\n\nuniform mat4 PVM; \n\nvoid main()\n{\n\ngl_Position = PVM * vec4(position, 1.0);\nColor = incolor; \ntexCoord = texcoord; \nsunlightLevel = sunlightlevel;\naoLevel = aolevel;\n}";
//const char* fragmentShaderSource = "\n#version 150\nin vec3 texCoord; \nin vec3 Color; \nin float sunlightLevel;\nin float aoLevel;\n\nout vec4 outColor;\n\nuniform sampler2DArray tex; \nuniform float sunlight;\n\nvoid main()\n{\n\noutColor = texture(tex, texCoord) * (clamp(max(vec4(Color, 1.0) , vec4((sunlight*sunlightLevel), (sunlight*sunlightLevel),(sunlight*sunlightLevel), 1.0)), vec4(0.05,0.05,0.05,1.0), vec4(0.8,0.8,0.8, 1.0)) + vec4((aoLevel*0.1), (aoLevel*0.1), (aoLevel*0.1), 0.0)) ;\n\n}";
const char* fragmentShaderSource = "\n#version 150\nin vec3 texCoord; \nin vec3 Color; \nin float sunlightLevel;\nin float aoLevel;\n\nout vec4 outColor;\n\nuniform sampler2DArray tex; \nuniform float sunlight;\n\nvoid main()\n{\n\noutColor = vec4((aoLevel*0.1), (aoLevel*0.1), (aoLevel*0.1), 1.0);\n\n}";

const char* vertexShaderSourceSky = "\n#version 150 \n\nin vec2 texcoord; \nin vec3 position; \nout vec2 texCoord; \nuniform mat4 PVM; \n\nvoid main()\n{\n\ngl_Position = PVM * vec4(position, 1.0);\ntexCoord = texcoord;}";
//const char* fragmentShaderSourceSky = "\n#version 150\nin vec2 texCoord; \nout vec4 outColor;\n\nuniform sampler2D tex; uniform float sunlight;\nvoid main()\n{\n\noutColor = texture(tex, texCoord) * vec4(sunlight, sunlight, sunlight, 1);\n\n}";
const char* fragmentShaderSourceSky = "\n#version 150\nin vec2 texCoord; \nout vec4 outColor;\n\nuniform sampler2D tex; uniform float sunlight;\nvoid main()\n{\n\noutColor = texture(tex, texCoord);\n\n}";

const char* vertexShaderSource2D = "#version 150\nin vec2 texcoord; in vec2 position; out vec2 texCoord;\n void main(){gl_Position = vec4(position, 0.0, 1.0); texCoord = texcoord;}";
const char* fragmentShaderSource2D = "#version 150\nin vec2 texCoord; \nout vec4 outColor;\nuniform sampler2D tex;\nvoid main()\n{\noutColor = texture(tex, texCoord);\n}";

const char* boxVertexShaderSource = "\n#version 150 \n\nin vec3 position; \n in vec3 incolor;\n out vec3 outcolor;\nuniform mat4 PVM; \n\nvoid main()\n{\n\ngl_Position = PVM * vec4(position, 1.0); outcolor = incolor;}";
const char* boxFragmentShaderSource = "\n#version 150 \n in vec3 outcolor; \n out vec4 outColor;\n\nvoid main()\n{\n\noutColor = vec4(outcolor, 1.0);\n\n}";
