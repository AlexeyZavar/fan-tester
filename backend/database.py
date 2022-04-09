import uuid

from sqlalchemy import create_engine, Column, Integer, String, Text, Boolean, PickleType, Float
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy_serializer import SerializerMixin

Base = declarative_base()


class Benchmark(Base, SerializerMixin):
    __tablename__ = 'Benchmarks'

    id = Column('id', Text(length=36), unique=True, primary_key=True, default=lambda: str(uuid.uuid4()))
    name = Column('name', String, nullable=False, unique=True)
    soft_start = Column('soft_start', Boolean, nullable=False)
    performed_on = Column('performed_on', Integer, nullable=False)
    data = Column('data', PickleType, nullable=False)

    def __str__(self):
        return self.name


engine = create_engine('sqlite:///data.db', echo=False, connect_args={'check_same_thread': False})  # connect to db
Base.metadata.create_all(engine)  # run init

DBSession = sessionmaker(autoflush=False, bind=engine)
