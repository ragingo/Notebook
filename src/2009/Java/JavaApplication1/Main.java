
package javaapplication1;
import java.*;
import java.text.SimpleDateFormat;
import java.util.*;

public class Main
{
    public static void main(String[] args)
	{
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
		System.out.println(DateTime.Now());
    }
}


final class DateTime extends Date
{
	private static GregorianCalendar _calendar;
	private final static String SHORT_DATETIME_FORMAT  = "yyyy/MM/dd HH:mm:ss";
	private final static String LONG_DATETIME_FORMAT = "yyyy年MM月dd日 HH時mm分ss秒";

	public DateTime()
	{
		_calendar = new GregorianCalendar();
	}
	public DateTime(Date date)
	{
		this();
		_calendar.setTime(date);
	}

	public final static DateTime Now()
	{
		return new DateTime();
	}

	@Override
	public String toString()
	{
		return new SimpleDateFormat(SHORT_DATETIME_FORMAT).format(_calendar.getTime());
	}
	public String toString(String format)
	{
		return new SimpleDateFormat(format).format(_calendar.getTime());
	}
}