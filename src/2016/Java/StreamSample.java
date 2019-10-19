import java.util.*;
import java.util.stream.*;
import java.time.*;
import java.time.format.*;
import java.lang.*;
import java.text.*;


public class StreamSample {

    public static void main(String[] args) {

        final Calendar birthDate = new GregorianCalendar(2016, 10, 8);
        int year = birthDate.get(Calendar.YEAR);

        // 2016 ~ 2115
        IntStream.range(year, year + 10)
            .mapToObj(y - >
                // 1 ~ 12
                IntStream.range(1, 13)
                .mapToObj(m - >
                    // 1 ~ (28~31)
                    IntStream.range(1, YearMonth.of(y, m).lengthOfMonth())
                    .mapToObj(d - >
                        new GregorianCalendar(y, m, d)
                    )
                    .collect(Collectors.toList())
                )
                .flatMap(x - > x.stream()) // List<List<>> to List<>
                .collect(Collectors.toList())
            )
            .flatMap(x - > x.stream()) // List<List<>> to List<>
            .filter(cal - >
                cal.get(Calendar.MONTH) == birthDate.get(Calendar.MONTH) &&
                cal.get(Calendar.DATE) == birthDate.get(Calendar.DATE)
            )
            .collect(Collectors.groupingBy(cal - > cal.get(Calendar.DAY_OF_WEEK)))
            .entrySet()
            .stream()
            .map(map - >
                DayOfWeek.of(map.getKey()).getDisplayName(TextStyle.SHORT, Locale.ENGLISH) +
                " : " +
                map.getValue()
                .stream()
                //.map(cal -> new SimpleDateFormat("yyyy").format(cal.getTime()))
                .map(cal - > String.valueOf(cal.get(Calendar.YEAR)))
                .collect(Collectors.joining(", ", "[", "]"))
            )
            .forEach(System.out::println);
    }

}
