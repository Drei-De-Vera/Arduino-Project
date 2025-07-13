void serialCommand() {
  String cmd = "";

  if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
  }

  if (cmd.length() <= 0) {
    return;
  }
  // DRY MATERIALS
  else if (cmd.startsWith("cof")) {
    Serial.println("===COFFEE===");
    digitalWrite(COFFEE, 0);
    delay(3000);
    digitalWrite(COFFEE, 1);
    Serial.println("===COFFEE DONE===");
  } else if (cmd.startsWith("shred")) {
    Serial.println("===SHREDDER===");
    digitalWrite(SHREDDER, 0);
    // delay(5000);
    // digitalWrite(SHREDDER, 1);
    // Serial.println("===SHREDDER DONE===");
  } else if (cmd.startsWith("soff")) {
    digitalWrite(SHREDDER, 1);
    Serial.println("===SHREDDER DONE===");
  } else if (cmd.startsWith("agar")) {
    Serial.println("===AGAR===");
    digitalWrite(AGAR, 0);
    delay(3000);
    digitalWrite(AGAR, 1);
    Serial.println("===AGAR DONE===");
  } else if (cmd.startsWith("star")) {
    Serial.println("===STARCH===");
    digitalWrite(STARCH, 0);
    delay(3000);
    digitalWrite(STARCH, 1);
    Serial.println("===STARCH DONE===");
  }
  // WET MATERIALS
  else if (cmd.startsWith("gly")) {
    Serial.println("===GLYCERIN===");
    digitalWrite(GLYCERIN, 0);
    delay(500);
    digitalWrite(GLYCERIN, 1);
    Serial.println("===GLYCERIN DONE===");
  } else if (cmd.startsWith("vin")) {
    Serial.println("===VINEGAR===");
    digitalWrite(VINEGAR, 0);
    delay(500);
    digitalWrite(VINEGAR, 1);
    Serial.println("===VINEGAR DONE===");
  } else if (cmd.startsWith("wat")) {
    Serial.println("===WATER===");
    digitalWrite(WATER, 0);
    delay(1000);
    digitalWrite(WATER, 1);
    Serial.println("===WATER DONE===");
  }
  // MIXING COMMAND
  else if (cmd.startsWith("mix")) {
    Serial.println("===MIXER===");
    digitalWrite(MIXER, 0);
    // delay(3000);
    // digitalWrite(MIXER, 1);
    // Serial.println("===MIXER DONE===");
  } else if (cmd.startsWith("moff")) {
        digitalWrite(MIXER, 1);
    Serial.println("===MIXER DONE===");
  }
  // SCALE COMMAND
  else if (cmd.startsWith("scin")) {
    Serial.println("===SCALE_IN===");
    digitalWrite(SCALE_IN, 0);
    delay(30000);
    digitalWrite(SCALE_IN, 1);
    Serial.println("===SCALE_IN DONE===");
  } else if (cmd.startsWith("scout")) {
    Serial.println("===SCALE_OUT===");
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
  } else if (cmd.startsWith("hoff")) {
    Serial.println("===OFF HEATER IN MIXER===");
    digitalWrite(HEATER1, 1);
   }else if (cmd.startsWith("f")) {
    Serial.println("===TURN ON FAN===");
    digitalWrite(FAN, 1);
  } else if (cmd.startsWith("of")) {
    Serial.println("===TURN OFF FAN===");
    digitalWrite(FAN, 0);
  } else if (cmd.startsWith("s")) {
    shredding();
  }
  // TEST EACH PHASE
  // else if (cmd.startsWith("A")) {
  //   shredTest();
  // }else if (cmd.startsWith("B")) {
  //   dispenseTest();
  // }else if (cmd.startsWith("C")) {
  //   wetTest();
  // }else if (cmd.startsWith("D")) {
  //   moldingTesting();
  // }else if (cmd.startsWith("E")) {
  //   moldOutTest();
  // }
}