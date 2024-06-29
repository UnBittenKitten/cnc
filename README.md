# cnc
CNC is a console application that transforms numbers between bases ranging from base 2 to base 64 made in C.

It supports transforming multiple numbers at the same time, numbers with a decimal point, and can also convert numbers to multiple numeric bases at the same time with the special word ALL.

To use it you simply type cnc followed by the base of the numbers you want to transform, followed by the base you want to transform to, followed by any amount of numbers you desire to be converted, and prints each number in the order you typed them, for example:

>cnc 10 2 121 32 67 12

Transforms the numbers 121, 32, 67 and 12 from base 10 to base 2 and outputs the following:

>1111001 100000 1000011 1100

CNC can detect if the numbers you typed have characters that aren't allowed in the numeric base you chose, for example:

>cnc 8 10 77 81 3

Outputs the following:

>63 NotValid 3

It transformed the numbers 77 and 3 correctly but detected that the number 81 is not a valid base8 number.

***

CNC can convert to multiple numeric bases if instead of writing a valid number you type all in the second base.

>cnc 10 all 14 1238

Outputs the following (it converts the numbers 14 and 1238 to the bases 2, 8, 10 and 16):


<blockquote>
  <ul style='list-style-type: none; padding:0'>
    <li>1110_(2)</li>
    <li>16_(8)</li>
    <li>14_(10)</li>
    <li>e_(16)</li>
  </ul>
  <ul style='list-style-type: none; padding:0'>
    <li>10011010110_(2)</li>
    <li>2326_(8)</li>
    <li>1238_(10)</li>
    <li>4d6_(16)</li>
  </ul>
</blockquote>

You can easily modify to what bases convert when using the all keyword.

***

CNC also supports multiple precision numbers, so it can handle numbers as big as your computer can handle thanks to the GMP library.

>cnc 16 10 71263fd1264df12a162319828dfca215e

Outputs the following:

>2406421842666647506439997294072568619358

***

CNC can work with really big numbers and decimal ones at the same time.

>cnc 32 10 817283764123123895.123837754

Outputs the following:

>310961048213010413577675045.03330240326351940995

***

