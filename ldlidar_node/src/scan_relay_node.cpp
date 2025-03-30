#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

class ScanRelayNode : public rclcpp::Node
{
public:
    ScanRelayNode() : Node("scan_relay_node")
    {
        // Create a subscription to the source topic
        subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "/ldlidar_node/scan", 10,
            std::bind(&ScanRelayNode::scan_callback, this, std::placeholders::_1));

        // Create a publisher to the target topic
        publisher_ = this->create_publisher<sensor_msgs::msg::LaserScan>("/scan", 10);

        RCLCPP_INFO(this->get_logger(), "ScanRelayNode started, relaying /ldlidar_node/scan to /scan");
    }

private:
    void scan_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr msg)
    {
        // Republish the message to /scan
        publisher_->publish(*msg);
        RCLCPP_INFO(this->get_logger(), "Relayed LaserScan message to /scan");
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr publisher_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ScanRelayNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}