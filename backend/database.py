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
    thrust_amperes = Column('thrust_amperes', PickleType, nullable=False)
    thrust_power = Column('thrust_power', PickleType, nullable=False)
    efficiency = Column('efficiency', PickleType, nullable=False)
    warmup_time = Column('warmup_time', Float, nullable=False)
    max_amperes = Column('max_amperes', Float, nullable=False)
    max_voltage = Column('max_voltage', Float, nullable=False)
    min_voltage = Column('min_voltage', Float, nullable=False)
    max_power = Column('max_power', Float, nullable=False)
    max_thrust = Column('max_thrust', Float, nullable=False)
    data = Column('data', PickleType, nullable=False)

    def __str__(self):
        return self.name


engine = create_engine('sqlite:///data.db', echo=False, connect_args={'check_same_thread': False})  # connect to db
Base.metadata.create_all(engine)  # run init

DBSession = sessionmaker(autoflush=False, bind=engine)
