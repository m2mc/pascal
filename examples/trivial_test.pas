program hello_world;

function foo : boolean
begin
    bar(2, 1) 
end;

function bar(a, b : integer) : integer
var x : integer;
begin
    x := 100000;
    while x > 0 do x := x - 1 - 2
end;

function inc(a : integer) : void
begin
    a := a + 1
end;

var i, j : integer;
var a, b : boolean;

begin
    i := 2;
    i := 1 + 3;
    i := i + j;
    j := i + j;

    if i = 3 then
    begin

    end;

    if a then
    begin

    end;

    foo();
    i := 5;
    write(i);
    inc(i);
    write(i)
end