void serialCommand() {
  String cmd = "";

  if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
  }

  if (cmd.length() <= 0) {
    return;
  }
  // DRY MATERIALS - Coffee, Plastic(shred), Agar-agar, starch
  else if (cmd.startsWith("c0")) {
    digitalWrite(COFFEE, 0);
    Serial.println("===COFFEE ON===");
  } 
  else if (cmd.startsWith("c1")) {
    digitalWrite(COFFEE, 1);
    Serial.println("===COFFEE OFF===");
  } 
  else if (cmd.startsWith("s0")) {
    digitalWrite(SHREDDER, 0);
    Serial.println("===SHREDDER ON===");
  } 
  else if (cmd.startsWith("s1")) {
    digitalWrite(SHREDDER, 1);
    Serial.println("===SHREDDER OFF===");
  } 
  else if (cmd.startsWith("a0")) {
    digitalWrite(AGAR, 0);
    Serial.println("===AGAR ON===");
  } else if (cmd.startsWith("a1")) {
    digitalWrite(AGAR, 1);
    Serial.println("===AGAR OFF===");
  } 
  else if (cmd.startsWith("st0")) {
    digitalWrite(STARCH, 0);
    Serial.println("===STARCH ON===");
  } 
  else if (cmd.startsWith("st1")) {
    digitalWrite(STARCH, 1);
    Serial.println("===STARCH OFF===");
  }
  // WET MATERIALS - GLYCERIN, VINEGAR, WATER
  else if (cmd.startsWith("g0")) {
    Serial.println("===GLYCERIN ON===");
    digitalWrite(GLYCERIN, 0);
    delay(18000); //18000 / 3000 = 6
    digitalWrite(GLYCERIN, 1);
    Serial.println("===GLYCERIN OFF===");
  } else if (cmd.startsWith("v0")) {
    Serial.println("===VINEGAR ON===");
    digitalWrite(VINEGAR, 0);
    delay(5000); //5000 / 100 = 50
    digitalWrite(VINEGAR, 1);
    Serial.println("===VINEGAR OFF===");
  } else if (cmd.startsWith("w0")) {
    Serial.println("===WATER ON===");
    digitalWrite(WATER, 0);
    delay(14000);  //14000 / 100 = 140
    digitalWrite(WATER, 1);
    Serial.println("===WATER OFF===");
  }
  // MIXING COMMAND
  else if (cmd.startsWith("m0")) {
    digitalWrite(MIXER, 0);
    Serial.println("===MIXER ON===");
  } else if (cmd.startsWith("m1")) {
    digitalWrite(MIXER, 1);
    Serial.println("===MIXER OFF===");
  }
  // SCALE/ACTUATOR/LOADCELL
  else if (cmd.startsWith("scin")) {
    Serial.println("===SCALE_IN RUNNING===");
    digitalWrite(SCALE_IN, 0);
    delay(30000);
    digitalWrite(SCALE_IN, 1);
    Serial.println("===SCALE_IN DONE===");
  } else if (cmd.startsWith("scout")) {
    Serial.println("===SCALE_OUT RUNNING===");
    digitalWrite(SCALE_OUT, 0);
    delay(30000);
    digitalWrite(SCALE_OUT, 1);
    Serial.println("===SCALE_OUT DONE===");
  }
  // DROP MATERIALS
  else if (cmd.startsWith("aopen")) {
    Serial.println("===ACT_OPEN===");
    digitalWrite(ACT_OPEN, 0);
    delay(30000);
    digitalWrite(ACT_OPEN, 1);
    Serial.println("===ACT_OPEN DONE===");
  } else if (cmd.startsWith("aclose")) {
    Serial.println("===ACT_CLOSE===");
    digitalWrite(ACT_CLOSE, 0);
    delay(30000);
    digitalWrite(ACT_CLOSE, 1);
    Serial.println("===ACT_CLOSE DONE===");
  }
  // MOLDING COMMAND
  else if (cmd.startsWith("min")) {
    Serial.println("===MOLD_IN===");
    digitalWrite(MOLD_IN, 0);
    delay(35000);
    digitalWrite(MOLD_IN, 1);
    Serial.println("===MOLD_IN DONE===");
  } else if (cmd.startsWith("mout")) {
    Serial.println("===MOLD_OUT===");
    digitalWrite(MOLD_OUT, 0);
    delay(35000);  // 90secs
    digitalWrite(MOLD_OUT, 1);
    Serial.println("===MOLD_OUT DONE===");
  } else if (cmd.startsWith("mold")) {
    if (digitalRead(LMS_MOLD) == HIGH) {
      Serial.println("===MOLDING===");
      digitalWrite(MOLDING, 0);
      while (digitalRead(LMS_MOLD) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(MOLDING, 1);
      Serial.println("===STOP MOLDING===");
    }
  }
  // HEATER COMMAND
  else if (cmd.startsWith("h1")) {
    Serial.println("===ON HEATER IN MIXER===");
    digitalWrite(HEATER1, 0);
  } else if (cmd.startsWith("h0")) {
    Serial.println("===OFF HEATER IN MIXER===");
    digitalWrite(HEATER1, 1);
  } 
}