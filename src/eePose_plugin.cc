#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "sensor_msgs/JointState.h"

#include <cstdio>

namespace gazebo
{
  class eePosePlugin : public ModelPlugin
  {
    public: eePosePlugin() {}
    /// \brief The load function is called by Gazebo when the plugin is
    /// inserted into simulation
    /// \param[in] _model A pointer to the model that this plugin is
    /// attached to.
    /// \param[in] _sdf A pointer to the plugin's SDF element.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      // Initialize ros, if it has not already bee initialized.
      if (!ros::isInitialized())
      {
        int argc = 0;
        char **argv = NULL;
        ros::init(argc, argv, "eePosePlugin", ros::init_options::NoSigintHandler);
      }
      // Create our ROS node. This acts in a similar manner to
      // the Gazebo node
      this->rosNode.reset(new ros::NodeHandle("eePosePlugin"));

      // Create a named topic, and subscribe to it.
      ros::SubscribeOptions so =
        ros::SubscribeOptions::create<sensor_msgs::JointState>(
            "/joint_states",
            1,
            boost::bind(&eePosePlugin::OnRosMsg, this, _1),
            ros::VoidPtr(), &this->rosQueue);
      this->rosSub = this->rosNode->subscribe(so);

      // Spin up the queue helper thread.
      this->rosQueueThread = std::thread(std::bind(&eePosePlugin::QueueThread, this));

    }
    public: void OnRosMsg(const sensor_msgs::JointStateConstPtr &_joints)
    {

          // number of joints we are going to process is the larger of velocity or position
          size_t jointCount = std::min(_joints->name.size(), std::max(_joints->velocity.size(), _joints->position.size()));
          if (jointCount == 0)
          {
              ROS_ERROR("eePosePlugin: Must at least provide one position or velocity");
              return;
          }

          for (size_t i = 0; i < jointCount; ++i)
          {
              std::string jointName = _joints->name[i];
              double targetPosVal = _joints->position[i];
              std::cout << "\n joint" << i << "joint name" << jointName <<
                           "joint value" << targetPosVal << std::endl;

          }



    }
      /// \brief ROS helper function that processes messages
    private: void QueueThread()
    {
        static const double timeout = 0.01;
        while (this->rosNode->ok())
        {
          this->rosQueue.callAvailable(ros::WallDuration(timeout));
        }
    }
    public: void OnUpdate()
    {

    }
     /// \brief A node use for ROS transport
     private: std::unique_ptr<ros::NodeHandle> rosNode;

     /// \brief A ROS subscriber
     private: ros::Subscriber rosSub;

     /// \brief A ROS callbackqueue that helps process messages
     private: ros::CallbackQueue rosQueue;

     /// \brief A thread the keeps running the rosQueue
     private: std::thread rosQueueThread;

  };


  // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
  GZ_REGISTER_MODEL_PLUGIN(eePosePlugin)
}

