#ifndef PLANET_CHUNK
#define PLANET_CHUNK


#include "planet.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PlanetChunk {
public: 
    glm::vec3 chunkPosition;  
    // Data used to describe the position and orientation of each planet 
    std::vector<glm::vec3> planetPositions; 
    std::vector<glm::vec3> planetZ; 
    std::vector<glm::vec3> planetY; 

    // Hold a vector of planets :sunglasses:
    std::vector<Planet> planetList; 

    Shader *planetShader;

    PlanetChunk(Shader *gShader, glm::vec3 ChunkPosition){
        chunkPosition = ChunkPosition; 
        planetShader = gShader; 
    
        generatePlanetData(); 
    


        // Now we initialize the vertices for the planets        
    }

    void generatePlanetData(){
        planetPositions.clear(); 
        planetZ.clear(); 
        planetY.clear();
        

        srand(10000 * (int)chunkPosition.x + 10000*(int)chunkPosition.y + 100*chunkPosition.z ); 

        for (int i = 0; i < PLANETS_PER_CHUNK; i++){
            glm::vec3 position; 
            glm::vec3 zAxis; 
            glm::vec3 yAxis;
            position.x = 
        }
    }

}; 

#endif
