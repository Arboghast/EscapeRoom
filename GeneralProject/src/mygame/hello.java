package mygame;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

public class hello
{
	private ArrayList<String> CSVData;
	private int numColumns;
	
	
	public static void main(String[] args)
	{
		//Creates CSV tracking file
		File CSV = new File("EscapeRoomScoreSheet.csv");
		hello tracker =  new hello(CSV);
		
		//s
		
		//Run this line of code after you run the game, if you put it before running the game there will be no data to write into CSV
		//tracker.writeToCSV(name, Time);
		
		//Runs through the column of long times and searches for the shortest time and tells user
		ArrayList<Long> data = new ArrayList<Long>();
		data = (ArrayList<Long>) tracker.getLongData(1);
    	
    	Long min = data.get(0);
    	
    	for(int i=1; i<data.size(); i++)
    	{
    		if(data.get(i) < min)
    		{
    			min = data.get(i);
    		}
    	}
    	System.out.println("\nCurrent fastest time is " + min/1000 + "seconds");
	}
	public hello(File CSV)
	{
		Path pathToFile = Paths.get(CSV.getAbsolutePath());
		this.CSVData = new ArrayList<String>();
		this.numColumns = 0;
		
		try(BufferedReader br = Files.newBufferedReader(pathToFile))
		{
			String line = br.readLine();
			String[] attributes2 = line.split(",");
			this.numColumns = attributes2.length;
			
			while(line != null)
			{
				String[] attributes = line.split(",");
				
				if(attributes.length == this.numColumns)
				{
					for(String x : attributes)
					{
						CSVData.add(x);
					}
				}
				line = br.readLine();
			}
		}
		catch(IOException ioe)
		{
			ioe.printStackTrace();
		}
	}
	public void writeToCSV(String name, long time)
	{
		PrintWriter pw = null;
		
		try
		{
			pw = new PrintWriter(new File("EscapeRoomScoreSheet.csv"));
		}
		catch(FileNotFoundException e)
		{
			System.err.println(e);
		}
		StringBuilder sb = new StringBuilder();
		for(int i=1; i<this.CSVData.size()+1; i++)
		{
			if(i % this.numColumns == 0)
			{
				sb.append(this.CSVData.get(i-1) + "\n");
			}
			else
			{
				sb.append(this.CSVData.get(i-1) + ",");
			}
		}
		this.CSVData.add(name);
		this.CSVData.add(Long.toString(time));
		sb.append(name +","+ Long.toString(time) +"\n");
		pw.write(sb.toString());
		pw.close();
	}
	public List<Long> getLongData(int column)
	{
		ArrayList<Long> columnDataLong = new ArrayList<Long>();
		
		for(int i=column + this.numColumns; i<this.CSVData.size(); i = i + this.numColumns)
		{
			columnDataLong.add(Long.parseLong(this.CSVData.get(i)));
		}
		return columnDataLong;
	}
}
