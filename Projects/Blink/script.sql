create table profile
(
   id serial,
   name character varying,
   usepreaut boolean,
   usevisit boolean,
   usedweller boolean,
   usevisitant boolean,
   useuser boolean,
   useprofile boolean,
   ConfigureEmail boolean,
   tp integer,
   removed boolean
);

ALTER TABLE ONLY profile
    ADD CONSTRAINT profile_key PRIMARY KEY (id);

insert into profile( id, name, usepreaut, usevisit, usedweller, usevisitant, useuser, useprofile, configureemail, tp, removed ) values ( 1, 'ADMINISTRADOR', true, true, true, true, true, true, true, 1, false );



create table vuser
(
   id serial,
   name character varying,
   email character varying,
   contractsince date,
   password character varying,
   profile integer,
   loid    integer,
   removed boolean
);

ALTER TABLE ONLY vuser
    ADD CONSTRAINT vuser_key PRIMARY KEY (id);

insert into vuser ( id, email, password, profile ) values ( 1, 'marco','1ac61befdde2a266f91b6088478c535e', 1);

create table report
(
   id serial,
   name character varying,
   path character varying,
   menu character varying,
   datemode integer,
   sql character varying,
   sqlcombo character varying,
   permission character varying,
   visible  boolean
);

ALTER TABLE ONLY report
    ADD CONSTRAINT report_key PRIMARY KEY (id);

