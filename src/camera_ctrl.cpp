#include<camera_ctrl.hpp>

CallBackFunc CameraControl::m_func = {};

#ifdef  _DEBUG
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat4) {
          for (int i = 0; i < 4; ++i) {
                    for (int j = 0; j < 4; ++j)
                              os << "  " << mat4[i][j];
                    os << std::endl;
          }
          return os;
}
#endif // _DEBUG

CameraControl::CameraControl(GLFWwindow* window)
          :m_window(window)
{
          /*load member function to C style callback*/
          convertMemberToCallBack();

          /*init matrix*/
          updateCameraViewMatrix();
          updateProjectionMatrix();
}

CameraControl::~CameraControl()
{

}

void CameraControl::updateCameraViewMatrix()
{
          m_view = std::move(glm::lookAt(m_camera_position, m_world_center, m_up_direction));
}

void CameraControl::updateProjectionMatrix() 
{
          m_projection = std::move(glm::perspective(glm::radians(m_fov), static_cast<float>(m_width / m_height), m_near, m_far));
}

const glm::mat4& CameraControl::getCameraViewMatrix()
{
          return m_view;
}

const glm::mat4& CameraControl::getProjectionMatrix()
{
          return m_projection;
}

glm::vec2 CameraControl::get_cursor_position(GLFWwindow* window)
{
          double x_pos, y_pos;
          glfwGetCursorPos(window, &x_pos, &y_pos);

          return glm::vec2(
                    static_cast<float>(2 * x_pos / m_width - 1),
                    static_cast<float>(2 * (m_height - y_pos) / m_height - 1));
}

void  CameraControl::mouseButtonBeingPressedCallBack(GLFWwindow* window, int button, int action, int mods)
{
          if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                    m_cursor_orbit_last_pos =get_cursor_position(window);

#ifdef  _DEBUG
                    printf("OpenGL Orbit MouseButton CallBack: (x, y) = (%f, %f)\n", m_cursor_orbit_last_pos.x, m_cursor_orbit_last_pos.y);
#endif // _DEBUG
          }
          else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                    m_cursor_drift_last_pos = get_cursor_position(window);

#ifdef  _DEBUG
                    printf("OpenGL Drift MouseButton CallBack: (x, y) = (%f, %f)\n", m_cursor_drift_last_pos.x, m_cursor_drift_last_pos.y);
#endif // _DEBUG
          }
}

void CameraControl::cursorMovementCallBack(GLFWwindow* window, double xpos, double ypos)
{
          if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
          {
                    auto new_pos = get_cursor_position(window);
                    orbit(m_cursor_orbit_last_pos, new_pos);

#ifdef  _DEBUG
                    printf("OpenGL Orbit CursorMovement CallBack: delta = (%f, %f)\n",
                              (new_pos - m_cursor_orbit_last_pos).x,
                              (new_pos - m_cursor_orbit_last_pos).y
                    );
#endif // _DEBUG

                    m_cursor_orbit_last_pos = new_pos;                                 //update last position
          }
          else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
          {
                    auto new_pos = get_cursor_position(window);
                    drift(m_cursor_drift_last_pos, new_pos);

#ifdef  _DEBUG
                    printf("OpenGL Drift CursorMovement CallBack: delta = (%f, %f)\n",
                              (new_pos - m_cursor_drift_last_pos).x,
                              (new_pos - m_cursor_drift_last_pos).y
                    );
#endif // _DEBUG

                    m_cursor_drift_last_pos = new_pos;
          }
}

void CameraControl::keyboardButtonBeingPressCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
          if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
                    glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
                    glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
                    glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

                    m_last_keyboard_opertion = key;

                    pan();

#ifdef  _DEBUG
                    printf("OpenGL Pan KeyBoard Pressed CallBack: Key = (%d)\n", m_last_keyboard_opertion);
#endif // _DEBUG

          }
}

void CameraControl::orbit(glm::vec2 last_pos, glm::vec2 new_pos) 
{
          const auto focus_zero = glm::normalize(m_world_center - m_camera_position);        //camers points to zero point!
          const auto pitch_axis = glm::normalize(glm::cross(focus_zero, m_up_direction));      //pitch control
          const auto yaw_axis = glm::normalize(glm::cross(pitch_axis, focus_zero));              //yaw control

#ifdef  _DEBUG
          std::cout << "up_direction: " << m_up_direction.x << " " << m_up_direction.y << " " << m_up_direction.z << std::endl;
          std::cout << "camera_coordinate: " << m_camera_position.x << " " << m_camera_position.y << " " << m_camera_position.z << std::endl;
          std::cout << "pitch: " << pitch_axis.x << " " << pitch_axis.y << " " << pitch_axis.z << std::endl;
          std::cout << "yaw: " << yaw_axis.x << " " << yaw_axis.y << " " << yaw_axis.z << std::endl;
#endif // _DEBUG

          /*start to calculate rotate matrics*/
          const auto delta = new_pos - last_pos;                                                                          //rad
          const auto yaw_rotate = glm::rotate(glm::mat4x4(1), -delta.x, yaw_axis);                  //oppsite from user's X movement
          const auto pitch_rotate = glm::rotate(glm::mat4x4(1), delta.y, pitch_axis);

          m_camera_position = glm::vec3(yaw_rotate * pitch_rotate * glm::vec4(m_camera_position, 1));

          const auto absolute2pitch_cos = glm::dot(pitch_axis, m_world_absolute_y_axis);
          const auto absolute2yaw_sin = glm::asin(absolute2pitch_cos);

          const auto normal = glm::normalize(glm::cross(m_world_absolute_y_axis, pitch_axis));
          const float flip_cos = glm::dot(normal, focus_zero);

          glm::mat4 rotate_forcus_zero = glm::rotate(glm::mat4(1), absolute2yaw_sin, focus_zero);

          //if the object is upside down then consider downside as its rotate destionation
          rotate_forcus_zero *= glm::atan(flip_cos);       

          m_up_direction = std::move(glm::mat3(rotate_forcus_zero) * yaw_axis);

          /*update matrix*/
          updateCameraViewMatrix();
}

void CameraControl::drift(glm::vec2 last_pos, glm::vec2 new_pos)
{
          const auto focus_zero = glm::normalize(m_world_center - m_camera_position);        //camers points to zero point!
          const auto pitch_axis = glm::normalize(glm::cross(focus_zero, m_up_direction));      //pitch control
          const auto yaw_axis = glm::normalize(glm::cross(pitch_axis, focus_zero));              //yaw control

          /*start to calculate rotate matrics*/
          const auto delta = new_pos - last_pos;                                                                          //rad
          const auto yaw_rotate = glm::rotate(glm::mat4x4(1), -delta.x, yaw_axis);                  //oppsite from user's X movement
          const auto pitch_rotate = glm::rotate(glm::mat4x4(1), delta.y, pitch_axis);

          const auto cam_trans_center = glm::translate(glm::mat4x4(1), m_world_center - m_camera_position);
          const auto center_trans_cam = glm::translate(glm::mat4x4(1), m_camera_position - m_world_center);

          const auto  rotate_obj = center_trans_cam * yaw_rotate * pitch_rotate * cam_trans_center;

          m_world_center = glm::vec3(rotate_obj * glm::vec4(m_world_center, 1));

#ifdef  _DEBUG
          std::cout << "up_direction: " << m_up_direction.x << " " << m_up_direction.y << " " << m_up_direction.z << std::endl;
          std::cout << "camera_coordinate: " << m_camera_position.x << " " << m_camera_position.y << " " << m_camera_position.z << std::endl;
          std::cout << "pitch: " << pitch_axis.x << " " << pitch_axis.y << " " << pitch_axis.z << std::endl;
          std::cout << "yaw: " << yaw_axis.x << " " << yaw_axis.y << " " << yaw_axis.z << std::endl;
          std::cout << "center: " << m_world_center.x << " " << m_world_center.y << " " << m_world_center.z << std::endl;
#endif // _DEBUG

          const auto absolute2pitch_cos = glm::dot(pitch_axis, m_world_absolute_y_axis);
          const auto absolute2yaw_sin = glm::asin(absolute2pitch_cos);

          const auto normal = glm::normalize(glm::cross(m_world_absolute_y_axis, pitch_axis));
          const float flip_cos = glm::dot(normal, focus_zero);

          glm::mat4 rotate_forcus_zero = glm::rotate(glm::mat4(1), absolute2yaw_sin, focus_zero);

          //if the object is upside down then consider downside as its rotate destionation
          rotate_forcus_zero *= glm::atan(flip_cos);

          m_up_direction = std::move(glm::mat3(rotate_forcus_zero) * yaw_axis);

          /*update matrix*/
          updateCameraViewMatrix();
}

void CameraControl::pan()
{

}

void CameraControl::convertMemberToCallBack()
{
          /*handle left mouse button pressed*/
          m_func.leftButtonPressedCallBack = std::move(std::bind(&CameraControl::mouseButtonBeingPressedCallBack, this,
                    std::placeholders::_1, 
                    std::placeholders::_2, 
                    std::placeholders::_3, 
                    std::placeholders::_4
          ));

          /*moving cursor while left mouse button was pressed*/
          m_func.cursorPosMovementCallBack = std::move(std::bind(&CameraControl::cursorMovementCallBack, this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
          ));

          /*user pressed keyboard up down right left button to pan the object*/
          m_func.keyboardPressedCallBack = std::move(std::bind(&CameraControl::keyboardButtonBeingPressCallBack, this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4,
                    std::placeholders::_5
          ));
}

void CameraControl::registerCallBackFunctions()
{
          glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
           {
                     m_func.leftButtonPressedCallBack(window, button, action, mods);
          });

          glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
          {
                     m_func.cursorPosMovementCallBack(window, xpos, ypos);
          });

          glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
           {
                     m_func.keyboardPressedCallBack(window, key, scancode, action, mods);
           });
}