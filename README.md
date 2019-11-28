# A monitoring system of relationships between elements

This is a school project, for the course of data structures and algorithms 2018/2019.

## Description

- We want to implement a mechanism for monitoring relationships between entities (for example people) that change over time
- Imagine, for example, a social network, in which new userscan register, and existing users can delete their own account, become "friends" of other users, break the relationship of friendship, etc.
- The relationships between entities are not necessarily symmetrical. For example, Alice can be a "friend" of Bruno, but friendship is not reciprocated (Bruno is not Alice's friend)

In a more abstract way, the mechanism monitors the following phenomena:

- A new entity begins to be monitored
- A monitored entity stops being one
- A new relationship is established between 2 monitored entities
- A relationship existing between 2 monitored entities ceases to exist
- Each entity has an identifying name (for example "Alice", "Bruno", "Carlo")
- There can be different types of relationships between entities, each identified by a name (for example "amico_di", "segu", "coetaneo_di")
- Each relationship has a verse (for example, if Alice is "friend_of" Bruno, the verse of the relationship is from Alice a Bruno, therefore Bruno is the "receiver" of the relationship), and is not necessarily symmetrical
- Following a specific command, the system returns, for each report, the entity that "receives" more relationships (if there are more entities whose number of reports received is maximum, these come printed in ascending order of identification)
- The application must be optimized to manage a large number of entities and instances of relationships, but generally few types (identifiers) of relationship

## Restriction

Implementation in standard C language (with only libc) of a program that reads a sequence of commands from standard input, each corresponding to a change in the entities or relationships between entities and, when required, produces on standard output, for each type of monitored relationship, the identifier of the entity that is the receiver of the greatest number of instances of that relationship, and the number of reports that the entity receives

## Input/Output

- addent ⟨id_ent⟩
  - adds an entity identified by "id_ent" to the set of monitored entities; if the entity is already monitored, it does nothing
- delent ⟨id_ent⟩
  - eliminates the entity identified by "id_ent" from the set of monitored entities; delete all the reports whose "id_ent" does part (both in origin and destination)
- addrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩
  - adds a relation - identified by "id_rel" - between the "id_orig" and "id_dest" entities, where "id_dest" is the receiver of report. If the relationship between "id_orig" and "id_dest" already exists, or if at least one of the entities is not monitored, it does not nothing. Monitoring of the "id_rel" relationship type begins implicitly with the first "addrel" command concerning it.
- delrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩
  - eliminates the relationship identified by "id_rel" between the entities "id_orig" and "id_dest" (where "id_dest" is the receiver of report); if there is no "id_rel" relationship between "id_orig" and "id_dest(with "id_dest" as the receiver), it does nothing
- report
  - outputs the list of reports, reporting for each entity with the largest number of incoming reports, as explained below
- end
  - end of the command sequence

### Note

- The output of the report command is a sequence made as follows: ⟨id_rel1⟩ ⟨id_ent1⟩ ⟨n_rel1⟩; ⟨Id_rel2⟩ ⟨id_ent2⟩ ⟨n_rel2⟩; ...
- the output relationships are sorted in ascending order of identification
- if for a relationship type there are more entities that are recipients of the maximum number of relations, these are produced in ascending order of identification, for example: ⟨Id_rel1⟩ ⟨id_ent1_1⟩ ⟨id_ent1_2⟩ ⟨id_ent1_3⟩… ⟨n_rel1⟩;
- if all relations with a certain identifier are removed, it does not appear in the subsequent output of the report command
- if there are no relationships between the entities, the output is none (without quotes)
- The identification of the identifiers follows the ASCII character table, so the following order applies: - <1 <A <\_ <a
- The various parts of each command and each output sequence are separated by spaces
- The end command has no output
