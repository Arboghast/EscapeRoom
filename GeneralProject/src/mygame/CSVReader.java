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

public class CSVReader
{
	private ArrayList<String> CSVData;
	private int numColumns;
	
	
	public static void main(String[] args)
	{	
		//Creates CSV tracking file
		File CSV = new File("EscapeRoomScoreSheet.csv");
		CSVReader tracker =  new CSVReader(CSV);
		
		//Run this line of code after you run the game, if you put it before running the game there will be no data to write into CSV
		//tracker.writeToCSV(Parameter 1 is a String, Parameter 2 is an Int, Parameter 3 is an Int);
		
		//Testing the getRowData function, should return an arrayList of all strings with row information
		ArrayList<String> data = (ArrayList<String>) tracker.getRowData();
		
		for(int i=0; i<data.size(); i++)
		{
			System.out.println(data.get(i));
		}
	}
	public CSVReader(File CSV)
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
	public void writeToCSV(String name, int minutes, int seconds)
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
		this.CSVData.add(Integer.toString(minutes));
		this.CSVData.add(Integer.toString(seconds));
		sb.append(name +","+ Integer.toString(minutes) + "," + Integer.toString(seconds) + "\n");
		pw.write(sb.toString());
		pw.close();
	}
	public ArrayList<String> getRowData()
	{
		ArrayList<String> rowData = new ArrayList<String>();
		
		for(int i=0; i<(this.CSVData.size()-3)/4; i++)
		{
			String name = getNameData().get(i);
			int minutes = getMinutesData().get(i);
			int seconds = getSecondsData().get(i);
			
			rowData.add("Player " +name+ " has completed the game in " +minutes+ " minutes and " +seconds+" seconds.");
		}
		return rowData;
	}
	public List<String> getNameData()
	{
		return getStringData(0);
	}
	public List<Integer> getMinutesData()
	{
		return getIntegerData(1);
	}
	public List<Integer> getSecondsData()
	{
		return getIntegerData(2);
	}
	public List<String> getStringData(int column)
	{
		ArrayList<String> columnDataString = new ArrayList<String>();
		
		for(int i=column + this.numColumns; i<this.CSVData.size(); i = i + this.numColumns)
		{
			columnDataString.add((this.CSVData.get(i)));
		}
		return columnDataString;
	}
	public List<Integer> getIntegerData(int column)
	{
		ArrayList<Integer> columnDataInteger = new ArrayList<Integer>();
		
		for(int i=column + this.numColumns; i<this.CSVData.size(); i = i + this.numColumns)
		{
			columnDataInteger.add(Integer.parseInt(this.CSVData.get(i)));
		}
		return columnDataInteger;
	}
}
