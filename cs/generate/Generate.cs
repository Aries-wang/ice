// **********************************************************************
//
// Copyright (c) 2003
// ZeroC, Inc.
// Billerica, MA, USA
//
// All Rights Reserved.
//
// Ice is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// **********************************************************************

using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Threading;

namespace Generate
{
    class Generate
    {
	static void Main(string[] args)
	{
	    string progName = AppDomain.CurrentDomain.FriendlyName;
	    if(args.Length != 3)
	    {
		Console.Error.WriteLine("usage: {0} solution_dir project_dir project_name", progName);
		Environment.Exit(1);
	    }

	    string solDir = args[0];
	    string projDir = args[1];
	    string projName = args[2];

	    string iceHome = Environment.GetEnvironmentVariable("ICE_HOME");

	    Directory.SetCurrentDirectory(projDir);

	    const string slicePat = @"*.ice";
	    string sliceDir = projDir;
	    string[] sliceFiles = Directory.GetFiles(projDir, slicePat);
	    string includes = "";
	    if(sliceFiles.Length == 0)
	    {
		sliceDir = Path.Combine(Path.Combine(solDir, "slice"), projName);
		if(Directory.Exists(sliceDir))
		{
		    sliceFiles = Directory.GetFiles(sliceDir, slicePat);
		    includes = "-I" + Path.Combine(solDir, "slice");
		    if(sliceFiles.Length == 0)
		    {
			if(iceHome != null)
			{
			    sliceDir = Path.Combine(Path.Combine(iceHome, "slice"), projName);
			    sliceFiles = Directory.GetFiles(sliceDir, slicePat);
			    includes = "-I" + Path.Combine(iceHome, "slice");
			}
		    }	    
		}
	    }
	    if(sliceFiles.Length == 0)
	    {
		Console.Error.WriteLine(progName + ": no Slice files found");
		Environment.Exit(1);
	    }

	    string slice2cs = iceHome == null ? "slice2cs" : Path.Combine(Path.Combine(iceHome, "bin"), "slice2cs");
	    string outputDir = Path.Combine(projDir, "generated");
	    string cmdArgs = "--ice -I. " + includes + " --output-dir " + outputDir;

	    bool needCompile = false;
	    foreach(string sliceFile in sliceFiles)
	    {
		DateTime sliceTime = File.GetLastWriteTime(sliceFile);
		string csFile = Path.Combine(outputDir, Path.ChangeExtension(Path.GetFileName(sliceFile), ".cs"));
		if(!File.Exists(csFile) || sliceTime > File.GetLastWriteTime(csFile))
		{	  
		    cmdArgs += " " + sliceFile;
		    Console.Out.WriteLine(Path.GetFileName(sliceFile));
		    needCompile = true;
		}
	    }

	    if(needCompile)
	    {
		ProcessStartInfo info = new ProcessStartInfo(slice2cs, cmdArgs);
		info.CreateNoWindow = true;
		info.UseShellExecute = false;
		info.RedirectStandardOutput = true;
		info.RedirectStandardError = true;
		p = Process.Start(info);
		Thread t1 = new Thread (new ThreadStart(RedirectStandardOutput));
		Thread t2 = new Thread(new ThreadStart(RedirectStandardError));
		t1.Start();
		t2.Start();
		p.WaitForExit();
		t1.Join();
		t2.Join();
		Environment.Exit(p.ExitCode);
	    }
	    Environment.Exit(0);
	}

	static volatile Process p;

	private static void RedirectStandardOutput()
	{
	    string output = p.StandardOutput.ReadToEnd();
	    Console.Out.Write(output);
	    Console.Out.Flush();
	}

	private static void RedirectStandardError()
	{
	    string output = p.StandardError.ReadToEnd();
	    Console.Error.Write(output);
	    Console.Error.Flush();
	}
    }
}	    
