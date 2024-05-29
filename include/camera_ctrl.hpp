#pragma once
#ifndef _CAMERA_CTRL_HPP_
#define _CAMERA_CTRL_HPP_
#include<glad/glad.h>
#include<glfw/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include<functional>
#include<vector>
#include<map>

#ifdef  _DEBUG
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat4);
#endif

struct CallBackFunc {
          using LeftButtonPressed = void(GLFWwindow* window, int button, int action, int mods);
          using CursorMovement = void(GLFWwindow* window, double xpos, double ypos);
          using KeyBoardPressed = void(GLFWwindow* window, int key, int scancode, int action, int mods);

          std::function< LeftButtonPressed> leftButtonPressedCallBack;              //leftButtonPushedCallBack
          std::function < CursorMovement > cursorPosMovementCallBack;         //when left button is pressed then move cursor
          std::function<KeyBoardPressed> keyboardPressedCallBack;                 //keyboard up down left right  button
};

/*
* Left Mouse Button                 : Left Mouse Button is for orbit control
* Right Mouse Button               : Right Mouse Button is for drift control
* KeyBoard up,down,left,right :  is for pan control
*/

class CameraControl
{
public:
          CameraControl(GLFWwindow* window = nullptr);
          virtual ~CameraControl();

public:
          void updateCameraViewMatrix();
          void updateProjectionMatrix();
          const glm::mat4& getCameraViewMatrix();
          const glm::mat4& getProjectionMatrix();

public:
          void registerCallBackFunctions();

private:
          glm::vec2 get_cursor_position(GLFWwindow* window);

          /*CALLBACK functions*/
          void mouseButtonBeingPressedCallBack(GLFWwindow* window, int button, int action, int mods);
          void cursorMovementCallBack(GLFWwindow* window, double xpos, double ypos);
          void keyboardButtonBeingPressCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

          /*camera orbit opertion*/
          void orbit(glm::vec2 last_pos, glm::vec2 new_pos);

          /*camera drift opertion*/
          void drift(glm::vec2 last_pos, glm::vec2 new_pos);

          /*camera pan operation*/
          void pan();

          /*convert member function to callback func pointer*/
          void convertMemberToCallBack();

protected:
          /*deletegation*/
          GLFWwindow* m_window;

          int m_width = 1024;
          int m_height = 768;
          float m_near = 0.1f;
          float m_far = 100.0f;
          float m_fov = 40.0f;

          /*record previous cursor position*/
          glm::vec2 m_cursor_orbit_last_pos;                                          //handle mouse left key press event
          glm::vec2 m_cursor_drift_last_pos;                                          //handle mouse right key press event

          /*record last keyboard command*/
          int m_last_keyboard_opertion;                                 

          /**/
          glm::vec3 m_camera_position = glm::vec3(1, 1, 5);                  //where is my camera?
          glm::vec3 m_world_center = glm::vec3(0, 0, 0);                       //the world center where camera is focusing on!
          glm::vec3 m_up_direction = glm::vec3(0, 1, 0);                       //the up direction of the model
          glm::vec3 m_world_absolute_y_axis = glm::vec3(0, 1, 0);      //absolute and can not be changes

          glm::mat4 m_projection;                                                            //projection matrix
          glm::mat4 m_view;                                                                     //camera view matrix

private:
          static CallBackFunc m_func;
};

#endif // !__CAMERA_CTRL_HPP_