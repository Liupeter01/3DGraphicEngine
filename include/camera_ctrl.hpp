#pragma once
#ifndef _CAMERA_CTRL_HPP_
#define _CAMERA_CTRL_HPP_
#include<glad/glad.h>
#include<GLFW/glfw3.h>
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
          using WindowsSizeChanged = void(GLFWwindow* window, int width, int height);
          using MouseScroll = void(GLFWwindow * window, double xoffset, double yoffset);

          std::function< LeftButtonPressed> leftButtonPressedCallBack;              //leftButtonPushedCallBack
          std::function < CursorMovement > cursorPosMovementCallBack;         //when left button is pressed then move cursor
          std::function<KeyBoardPressed> keyboardPressedCallBack;                 //keyboard up down left right  button
          std::function<WindowsSizeChanged> windowsSizeChangedCallBack;   //user changed window size
          std::function<MouseScroll> mouseScrollCallBack;                                 //user scroll his/her mouse
};

/*
* Left Mouse Button                 : Left Mouse Button is for orbit control
* Right Mouse Button               : Right Mouse Button is for drift control
* KeyBoard up,down,left,right(WSAD) :  is for pan control
*/

/*
 * pan control(up,down,left,right)
 * WASD also supported
*/
enum class PanDirection {
          UP = GLFW_KEY_UP,
          DOWN = GLFW_KEY_DOWN,
          LEFT = GLFW_KEY_LEFT,
          RIGHT = GLFW_KEY_RIGHT
};

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
          void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
          void cursorMovementCallBack(GLFWwindow* window, double xpos, double ypos);
          void keyboardButtonBeingPressCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
          void windowsSizeChangedCallBack(GLFWwindow* window, int width, int height);

          /*camera orbit opertion*/
          void orbit(glm::vec2 last_pos, glm::vec2 new_pos);

          /*camera drift opertion*/
          void drift(glm::vec2 last_pos, glm::vec2 new_pos);

          /*camera pan operation*/
          void pan();

          /*camera zoom operation*/
          void zoom(float yoffset);

          /*convert member function to callback func pointer*/
          void convertMemberToCallBack();

protected:
          /*deletegation*/
          GLFWwindow* m_window;
             
          /*VIEW and PROJECTION Matrix*/
          int m_width = 1024;
          int m_height = 768;
          float m_near = 0.1f;
          float m_far = 100.0f;
          float m_fov = 40.0f;

          glm::vec3 m_camera_position = glm::vec3(1, 1, 5);                  //where is my camera?
          glm::vec3 m_world_center = glm::vec3(0, 0, 0);                       //the world center where camera is focusing on!
          glm::vec3 m_up_direction = glm::vec3(0, 1, 0);                       //the up direction of the model
          glm::vec3 m_world_absolute_y_axis = glm::vec3(0, 1, 0);      //absolute and can not be changes

          glm::mat4 m_projection;                                                            //projection matrix
          glm::mat4 m_view;                                                                     //camera view matrix

private:
          /*up,down,left,right pan offset parameter*/
          float m_pan_offset = 0.05f;
          PanDirection m_pan_control;

          /*zoom scale parameter*/
          float m_zoom_scale = 0.05f;

          /*record previous cursor position*/
          glm::vec2 m_cursor_orbit_last_pos;                                          //handle mouse left key press event
          glm::vec2 m_cursor_drift_last_pos;                                          //handle mouse right key press event

          static CallBackFunc m_func;
};

#endif // !__CAMERA_CTRL_HPP_
