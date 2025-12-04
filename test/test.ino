#include <AGVCoreNetwork.h>

// 1. Command handler function (called when commands arrive)
void onCommandReceived(const char* command, uint8_t source, uint8_t priority) {
  // source: 0 = web interface, 1 = serial monitor
  // priority: 0 = normal, 1 = emergency (STOP/ABORT)
  
  Serial.printf("[AGV] Processing command: %s (source=%d, priority=%d)\n", 
                command, source, priority);
  
  // Example: Emergency stop has highest priority
  if (priority == 1 && (strstr(command, "STOP") || strstr(command, "ABORT"))) {
    Serial.println("!!! EMERGENCY STOP ACTIVATED !!!");
    // Your emergency stop code here
  }
  
  // Example: Path commands
  if (strstr(command, "PATH:")) {
    Serial.printf("Executing path command: %s\n", command);
    // Your path execution code here
  }
  
  // Send status update back to interfaces
  String status = "Executing: ";
  status += command;
  agvNetwork.sendStatus(status.c_str());
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // 2. Initialize network system with device name and credentials
  agvNetwork.begin("factory_agv_01", "admin", "agv_secure_pass");
  
  // 3. Register command callback (connects communication to your AGV logic)
  agvNetwork.setCommandCallback(onCommandReceived);
  
  Serial.println("\n✅ AGV system ready!");
  Serial.println("🌐 Web interface: http://factory_agv_01.local");
  Serial.println("⌨️  Serial commands: START, STOP, PATH:1,1,3,2:ONCE, etc.");
  Serial.println("📱 Connect to 'AGV_Controller_Network' for initial setup");
}

void loop() {
  // 4. Nothing needed here! The library runs in its own FreeRTOS task on Core 0
  // Your Core 1 AGV control code would run here (motor control, sensors, etc.)
  
  // Optional: Send periodic status updates
  static unsigned long lastStatus = 0;
  if (millis() - lastStatus > 5000) {
    agvNetwork.sendStatus("AGV Ready - Idle");
    lastStatus = millis();
  }
  
  delay(100);
}