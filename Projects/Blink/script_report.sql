--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: report; Type: TABLE; Schema: public; Owner: dsm; Tablespace: 
--

CREATE TABLE report (
    id integer NOT NULL,
    name character varying,
    path character varying,
    menu character varying,
    datemode integer,
    sql character varying,
    permission character varying,
    visible boolean,
    combomode integer,
    sqlcombo character varying,
    wherecombo character varying
);


ALTER TABLE report OWNER TO dsm;

--
-- Name: report_id_seq; Type: SEQUENCE; Schema: public; Owner: dsm
--

CREATE SEQUENCE report_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE report_id_seq OWNER TO dsm;

--
-- Name: report_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: dsm
--

ALTER SEQUENCE report_id_seq OWNED BY report.id;


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: dsm
--

ALTER TABLE ONLY report ALTER COLUMN id SET DEFAULT nextval('report_id_seq'::regclass);


--
-- Data for Name: report; Type: TABLE DATA; Schema: public; Owner: dsm
--

COPY report (id, name, path, menu, datemode, sql, permission, visible, combomode, sqlcombo, wherecombo) FROM stdin;
1	Logins	Relatorios	Logins	1	select u.email "Usuário", date "Data", time "Hora", m.machine "Máquina" from login l inner join machine m on m.id = l.machineid inner join vuser u on u.id = l.userid  where date between '%1' and '%2' %3 order by l.id desc	useivisitant	t	1	select id, email from vuser	and l.userid = %1
2	Moradores	Relatorios	Moradores	0	select d.name "Nome", t.name "Torre", a.numero "Ap", dt.name "Tipo", d.email "email" from dweller d inner join tower t on t.id = d.tower inner join ap a on a.id = d.ap inner join dweller_type dt on dt.id = d.type %1 order by d.id	useivisitant	\N	1	select id, name from dweller_type	where dt.id = %1
\.


--
-- Name: report_id_seq; Type: SEQUENCE SET; Schema: public; Owner: dsm
--

SELECT pg_catalog.setval('report_id_seq', 2, false);


--
-- Name: report_key; Type: CONSTRAINT; Schema: public; Owner: dsm; Tablespace: 
--

ALTER TABLE ONLY report
    ADD CONSTRAINT report_key PRIMARY KEY (id);


--
-- PostgreSQL database dump complete
--

