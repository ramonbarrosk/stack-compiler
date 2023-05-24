program RecursivePrintNumber;

procedure RecursivePrintNumber(num: integer);
begin
  writeln(num);
  if num < 10 then
    RecursivePrintNumber(num+2);
end;

begin
  RecursivePrintNumber(5);
end.