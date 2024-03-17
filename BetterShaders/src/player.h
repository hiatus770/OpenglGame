#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "shader.h"
#include "object.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float MovementSpeed = 0.5;

enum Player_Movement
{
    FORWARD,
    BACKWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT,
    ROLL_LEFT,
    ROLL_RIGHT,
    RISE,
    FALL,
    PITCH_UP,
    PITCH_DOWN,
    YAW_LEFT,
    YAW_RIGHT
};

const glm::vec3 cameraOffset = glm::vec3(-3.0f, 1.0f, 0.0f); 

class Player
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 localUp;
    std::vector<float> vertices;
    std::vector<float> color;

    Player(std::vector<float> Vertices, glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Direction = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 LocalUp = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        position = Position;
        direction = Direction;
        localUp = LocalUp;
        vertices = Vertices;
    }

    void processKeyboard(Player_Movement dir, float deltaTime = 0.1)
    {
        float velocity = MovementSpeed * deltaTime;
        if (dir = FORWARD)
            position += velocity * direction;
        if (dir == BACKWARD)
            position -= MovementSpeed * direction;
        if (dir == ROLL_LEFT)
            localUp = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), direction)) * localUp;
        if (dir == ROLL_RIGHT)
            localUp = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), direction)) * localUp;
        if (dir == PITCH_UP)
        {
            direction = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::cross(direction, localUp)) * glm::vec4(direction, 1.0f));
        }
        if (dir == PITCH_DOWN)
        {
            direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), glm::cross(direction, localUp))) * direction;
        }

        if (dir == YAW_RIGHT)
        {
            direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-1.0f), localUp)) * direction;
        }

        if (dir == YAW_LEFT)
        {
            direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), localUp)) * direction;
        }

        // Strafing keys
        if (dir == STRAFE_LEFT)
        {
            position -= glm::normalize(glm::cross(direction, localUp)) * MovementSpeed;
        }
        if (dir == STRAFE_RIGHT)
        {
            position += glm::normalize(glm::cross(direction, localUp)) * MovementSpeed;
        }

        // Rise and Fall keys
        if (dir == RISE)
        {
            position += glm::normalize(localUp) * MovementSpeed;
        }
        if (dir == FALL)
        {
            position -= glm::normalize(localUp) * MovementSpeed;
        }
    }

    glm::vec3 getCameraPosition(){
        return position + cameraOffset; 
    }

    glm::vec3 getCameraUp(){
        return localUp + cameraOffset; 
    }

    // Return camera nonsense for the position of the camera, dont actually store the camrea here make a global one!
};

#endif