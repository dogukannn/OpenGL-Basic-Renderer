#include "Material.h"

bool ReadDataFromFile(
    const std::string& fileName, ///< [in]  Name of the shader file
    std::string& data)     ///< [out] The contents of the file
{
    std::fstream myfile;

    // Open the input 
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        std::string curLine;

        while (getline(myfile, curLine))
        {
            data += curLine;
            if (!myfile.eof())
            {
                data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

    return true;
}

GLuint createVS(std::string filename)
{
    std::string shaderSource;

    
    if (!ReadDataFromFile(filename, shaderSource))
    {
        std::cout << "Cannot find file name: " + filename << std::endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*)shaderSource.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = { 0 };
    glGetShaderInfoLog(vs, 1024, &length, output);
    printf("VS compile log: %s\n", output);

    return vs;
}

GLuint createFS(std::string filename)
{
    std::string shaderSource;

   
    if (!ReadDataFromFile(filename, shaderSource))
    {
        std::cout << "Cannot find file name: " + filename << std::endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*)shaderSource.c_str();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = { 0 };
    glGetShaderInfoLog(fs, 1024, &length, output);
    printf("FS compile log: %s\n", output);

    return fs;
}

GLuint createGS(std::string filename)
{
    std::string shaderSource;


    if (!ReadDataFromFile(filename, shaderSource))
    {
        std::cout << "Cannot find file name: " + filename << std::endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*)shaderSource.c_str();

    GLuint fs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = { 0 };
    glGetShaderInfoLog(fs, 1024, &length, output);
    printf("FS compile log: %s\n", output);

    return fs;
}

Material::Material()
{
}

void Material::initShaders(std::string vs_shader, std::string frag_shader)
{
    // Create the programs
    glprogramIndex = glCreateProgram();
    
    // Create the shaders for both programs
    GLuint vs1 = createVS(vs_shader);
    GLuint fs1 = createFS(frag_shader);
   
    // Attach the shaders to the programs
    glAttachShader(glprogramIndex, vs1);
    glAttachShader(glprogramIndex, fs1);

    // Link the programs
    glLinkProgram(glprogramIndex);
    GLint status;
    glGetProgramiv(glprogramIndex, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        std::cout << "Program link failed" << std::endl;
        exit(-1);
    }

    // Get the locations of the uniform variables from both programs  
    modelingMatrixLoc = glGetUniformLocation(glprogramIndex, "modelingMatrix");
    viewingMatrixLoc = glGetUniformLocation(glprogramIndex, "viewingMatrix");
    projectionMatrixLoc = glGetUniformLocation(glprogramIndex, "projectionMatrix");
    std::cout << projectionMatrixLoc << std::endl;
    eyePosLoc = glGetUniformLocation(glprogramIndex, "eyePos");
    std::cout << eyePosLoc << std::endl;
    iResloc = glGetUniformLocation(glprogramIndex, "iResolution");
    iTimeloc = glGetUniformLocation(glprogramIndex, "iTime");
    lineThicknessLoc = glGetUniformLocation(glprogramIndex, "lineThickness");
    std::cout << "line thickness loc " << lineThicknessLoc << std::endl;

    
}

Material::~Material()
{
}