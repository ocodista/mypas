program Teste;

    { Declarative scope }
	var i, j: integer;
	    x, y: real;
	    t, w: double;

	procedure A(n: integer);
		var abacaxi: integer;

		{ Imperative scope }
		begin
			abacaxi := n + 1;
		end;

	{ imperative scope }
	begin

        A(2);
	end.

	O resto é comentário

