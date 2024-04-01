#ifndef PLANET_H
#define PLANET_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

enum planetType
{
    SPHERE,
    SPHERE_RING,
    WACKY_AHH_PLANET_SHAPE
};

class Planet
{
public:
    Object *planetObj;
    glm::vec3 position;
    glm::vec3 zAxis; 
    glm::vec3 yAxis; 
    std::vector<float> color;
    glm::vec3 rotAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    std::vector<float> vertices;
    planetType type;

    Planet(Shader *gShader, glm::vec3 Position, std::vector<float> Color = {1.0f, 1.0f, 1.0f, 1.0f}, planetType PlanetType = SPHERE)
    {
        position = Position;
        color = Color;
        type = PlanetType;

        generateVertices();

        //
        planetObj = new Object(gShader, vertices, color);

        // We must generate those vertices tho :o
    }

    void generateVertices()
    {
        // We already know the type of planet it is so we have 3 cases
        if (type == SPHERE)
        {
            int ringPoints = 20;    // 10 pounds on each ring of the planet
            int ringRotations = 20; // How many rotations of the ring we gonna have
            float radius = 10;
            glm::vec3 ring[ringPoints];
            for (int i = 0; i < ringPoints; i++)
            {
                // parameter t value
                float t = 2 * M_PI * (i / (float)ringPoints);
                ring[i] = (glm::vec3(sinf(t) * radius, cosf(t) * radius, 0.0f));
                vertices.push_back(ring[i].x);
                vertices.push_back(ring[i].y);
                vertices.push_back(ring[i].z);
            }

            // Now rotate za ring along an axis, the y axis in fact!
            for (int i = 0; i < ringRotations; i++)
            {
                float theta = (i / (float)ringRotations) * (2 * M_PI);
                // Now rotate each point individually and see where it gets  you then do a bit of mogusing
                for (int j = 0; j < ringPoints; j++)
                {
                    // Now we rotate for each of the points
                    glm::vec3 tempPoint = ring[j];
                    tempPoint = glm::mat3(glm::rotate(glm::mat4(1.0f), (theta), glm::vec3(0.0f, 1.0f, 0.0f))) * tempPoint;

                    // vertices.push_back(ring[j].x);
                    // vertices.push_back(ring[j].y);
                    // vertices.push_back(ring[j].z);
                    vertices.push_back(tempPoint.x);
                    vertices.push_back(tempPoint.y);
                    vertices.push_back(tempPoint.z);
                }
            }

            ringRotations *= 2; 
            for (int i = 0; i < ringPoints; i++)
            {
                // Now we grooving
                float heightFromBottom = (i * radius) / ringPoints;
                // If Calculate the radius that u need to use
                float tempRadius = sqrtf(radius*radius-(fabs(radius - 2*(i * radius) / ringPoints))*(fabs(radius - 2*(i * radius) / ringPoints))); 
                // tempRadius = 1; 
                // Time to do some generatoin now
                for (int j = 0; j < ringRotations; j++)
                {
                    float t = (4 * M_PI) * (j / (float)(ringRotations));
                    glm::vec3 tempPoint = (glm::vec3((float)sinf(t) * tempRadius, 2*heightFromBottom-radius, (float)cosf(t) * tempRadius));

                    vertices.push_back(tempPoint.x);
                    vertices.push_back(tempPoint.y);
                    vertices.push_back(tempPoint.z);
                    
                    t = (4 * M_PI) * ((j+1) / (float)(ringRotations));
                    tempPoint = (glm::vec3((float)sinf(t) * tempRadius, 2*heightFromBottom-radius, (float)cosf(t) * tempRadius));

                    vertices.push_back(tempPoint.x);
                    vertices.push_back(tempPoint.y);
                    vertices.push_back(tempPoint.z);
                }
            }
            // Radius must also be defined :skull:
            // First we have the y which has a maximum of the -radius to +radius

            // Cool idea, we make one set of points then rotate it wowee
        }
    }

    void render(float deltaTime, glm::mat4 view, glm::mat4 projection)
    {

        planetObj->model = glm::translate(glm::mat4(1.0f), position);
        planetObj->render(view, projection, GL_LINES);
    }
};

#endif