program trivial_test;

function inc(a : integer) : void
begin
    a := a + 1
end;

var i : integer;

begin
    i := 5;
    write(i);
    inc(i);
    write(i);
    inc(42)     { error! }
end