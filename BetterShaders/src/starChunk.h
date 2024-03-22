#ifndef STARCHUNK_H
#define STARCHUNK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int STARS_PER_CHUNK = 1000;

class StarChunk
{
public:
    float chunkX;
    float chunkY;
    float chunkZ; 
    std::vector<glm::vec3> starsPositions;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int instanceVBO;
    Shader *starShader;

    StarChunk(float ChunkX, float ChunkY, float ChunkZ)
    {
        chunkX = ChunkX;
        chunkY = ChunkY;
        chunkZ = ChunkZ; 
    }

    void generateStarVertices()
    {
        srand(400 * (int)chunkZ + 1000 * (int)chunkX + (int)chunkY);
        
        for (int i = 0; i < STARS_PER_CHUNK; i++)
        {
            glm::vec3 position;
            position.x = (float)(rand() % (CHUNK_SIZE * 100)) / 100.0f;
            position.y = (float)(rand() % (CHUNK_SIZE * 100)) / 100;
            position.z = (float)(rand() % (CHUNK_SIZE * 100)) / 100;
            std::cout << position.x << std::endl; 
            starsPositions.push_back(position);
        }
    }

    void init(glm::vec3 pos)
    {
        chunkX = (float)pos.x; 
        chunkY = (float)pos.y;
        chunkZ = (float)pos.z; 

        generateStarVertices();
        // Create shader
        starShader = new Shader("/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/vertInstanced.vs", "/home/hiatus/Documents/OPENGLPROJECT/BetterShaders/src/shaders/fragInstanced.fs");

        // Now create VBO VAO nonsense stuff

        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * starsPositions.size(), starsPositions.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO); // Generate 1

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * starVertices.size(), starVertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glVertexAttribDivisor(1, 1);
    }

    void setCoordinate() {}

    void render()
    {
        starShader->use();
        starShader->setVec4("aColor", {1.0f, 0.50f, 0.0f, 1.0f});
        starShader->setMat4("projection", camera.getProjectionMatrix());
        starShader->setMat4("view", camera.getViewMatrix());
        starShader->setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(CHUNK_SIZE * chunkX, CHUNK_SIZE * chunkY, CHUNK_SIZE * chunkZ)));
        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_LINES, 0, starVertices.size(), STARS_PER_CHUNK);
        glBindVertexArray(0);
    }
};

#endif