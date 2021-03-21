program Teste;

    { Declarative scope }
	var i, j: integer;
	    x, y: real;
	    t, w: double;

	procedure A(n: integer);
		var abacaxi: integer;

		{ Imperative scope }
		begin
			abacaxi := n;
			if n <= 2 then
				abacaxi := n * 2
			else
				abacaxi := n / 2;
		end;

	{ imperative scope }
	begin
        A(2);
		A(4);
		A(6);
	end.

	O resto é comentário

