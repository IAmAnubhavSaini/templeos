Bool app_done;
I64 app_done_ack;

U0 Job1(I64)
{ //Compile Compiler
  TaskInitExt; //Enable file access
  Preempt(ON);	 //Off by default
  Silent(ON);
  while (!app_done) {
    Cd("::/TempleOS/Compiler");
    Cmp("Compiler");
  }
  lock {app_done_ack--;}
}

U0 Job2(I64)
{ //Log a file
  TaskInitExt; //Enable file access
  Preempt(ON);	 //Off by default
  while (!app_done) {
    AdamLog("%F","::/TempleOS/Demo/MultiCore/LoadTest.CPP");
    Yield;
  }
  lock {app_done_ack--;}
}

U0 Job3(I64)
{ //Log a file
  TaskInitExt; //Enable file access
  Preempt(ON);	 //Off by default
  while (!app_done) {
    Cd("::/TempleOS/Compiler");
    AdamLog("%F","Compiler.PRJ.Z");
    Yield;
  }
  lock {app_done_ack--;}
}

U0 Main()
{
  app_done=FALSE;
  "Running MultiCore Test...\n";
  Spawn(&Job1,,,1); //core #1
  Spawn(&Job2,,,1);
  Spawn(&Job3,,,1);
  PressAKey;
//We don't want to yank code from under MP cores.
  app_done_ack=3;
  app_done=TRUE;
  while (app_done_ack)
    Yield;
}

Main;
